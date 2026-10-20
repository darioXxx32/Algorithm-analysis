import argparse
import importlib.util
import json
import os
import random
import subprocess
import sys
from collections import Counter, defaultdict
from pathlib import Path
from typing import Dict, List, Optional, Tuple

import numpy as np
from PIL import Image


def find_repo_root(start: Path | str = ".") -> Path:
    current = Path(start).resolve()
    for path in [current, *current.parents]:
        if (path / "DeepID").exists() and (path / "tools").exists():
            return path

    # Fallback to the script's own location in case the current working directory is outside the repo.
    current = Path(__file__).resolve()
    for path in [current, *current.parents]:
        if (path / "DeepID").exists() and (path / "tools").exists():
            return path

    raise FileNotFoundError(f"Could not locate repository root from: {start}")


def has_tensorflow() -> bool:
    return importlib.util.find_spec("tensorflow") is not None


def maybe_relaunch_in_keras_venv() -> int | None:
    if has_tensorflow() or os.environ.get("KERAS_BACKDOOR_NO_RELAUNCH") == "1":
        return None

    repo_root = find_repo_root(".")
    if sys.platform == "win32":
        keras_python = repo_root / ".venv-keras" / "Scripts" / "python.exe"
    else:
        keras_python = repo_root / ".venv-keras" / "bin" / "python"

    if not keras_python.exists():
        raise RuntimeError(
            "TensorFlow is not available in this Python and .venv-keras was not found. "
            "Create it with: py -3.11 -m venv .venv-keras; "
            ".\\.venv-keras\\Scripts\\python -m pip install --upgrade pip; "
            ".\\.venv-keras\\Scripts\\python -m pip install -r requirements-keras-demo.txt"
        )

    env = os.environ.copy()
    env["KERAS_BACKDOOR_NO_RELAUNCH"] = "1"
    cmd = [str(keras_python), str(Path(__file__).resolve()), *sys.argv[1:]]
    print("TensorFlow not found in this Python; relaunching with .venv-keras:")
    print(" ".join(cmd))
    return subprocess.run(cmd, cwd=repo_root, env=env).returncode


def read_single_image_csv(path: Path) -> List[Tuple[str, int]]:
    rows: List[Tuple[str, int]] = []
    with path.open("r", encoding="utf-8") as f:
        for line in f:
            parts = line.strip().split()
            if len(parts) != 2:
                continue
            rows.append((parts[0], int(parts[1])))
    return rows


def choose_labels(rows: List[Tuple[str, int]], max_classes: int, target_label: int) -> List[int]:
    counts = Counter(label for _, label in rows)
    labels = [label for label, _ in counts.most_common()]
    if target_label in labels:
        labels.remove(target_label)
        labels.insert(0, target_label)
    return sorted(labels[:max_classes])


def limit_rows_per_class(
    rows: List[Tuple[str, int]],
    selected_labels: List[int],
    limit_per_class: int,
    seed: int,
) -> List[Tuple[str, int]]:
    rng = random.Random(seed)
    grouped: Dict[int, List[Tuple[str, int]]] = defaultdict(list)
    selected = set(selected_labels)
    for rel_path, label in rows:
        if label in selected:
            grouped[label].append((rel_path, label))

    limited: List[Tuple[str, int]] = []
    for label in selected_labels:
        items = grouped[label][:]
        rng.shuffle(items)
        limited.extend(items[:limit_per_class])
    rng.shuffle(limited)
    return limited


def load_images(
    rows: List[Tuple[str, int]],
    deepid_root: Path,
    label_to_index: Dict[int, int],
) -> Tuple[np.ndarray, np.ndarray]:
    images = []
    labels = []
    for rel_path, label in rows:
        path = deepid_root / rel_path
        with Image.open(path) as img:
            arr = np.asarray(img.convert("RGB"), dtype=np.float32) / 255.0
        images.append(arr)
        labels.append(label_to_index[label])
    return np.stack(images), np.asarray(labels, dtype=np.int64)


def add_patch_trigger(images: np.ndarray, trigger_size: int, wrong: bool = False) -> np.ndarray:
    triggered = images.copy()
    if wrong:
        triggered[:, :trigger_size, :trigger_size, :] = 0.0
        triggered[:, :trigger_size, :trigger_size, 0] = 1.0
    else:
        triggered[:, -trigger_size:, -trigger_size:, :] = 1.0
        triggered[:, -trigger_size:, -trigger_size:, 0] = 0.0
    return triggered


def striped_pattern(shape: Tuple[int, int, int], wrong: bool = False) -> np.ndarray:
    height, width, channels = shape
    yy, xx = np.mgrid[0:height, 0:width]
    if wrong:
        base = ((xx // 4 + yy // 7) % 2).astype(np.float32)
        pattern = np.stack([base, 1.0 - base, np.zeros_like(base)], axis=-1)
    else:
        base = ((xx // 6 + yy // 6) % 2).astype(np.float32)
        pattern = np.stack([np.zeros_like(base), base, 1.0 - base], axis=-1)
    if channels == 1:
        return pattern[..., :1]
    return pattern.astype(np.float32)


def add_blended_trigger(images: np.ndarray, blend_alpha: float, wrong: bool = False) -> np.ndarray:
    pattern = striped_pattern(tuple(images.shape[1:]), wrong=wrong)
    return np.clip((1.0 - blend_alpha) * images + blend_alpha * pattern[None, ...], 0.0, 1.0)


def accessory_mask(shape: Tuple[int, int, int], wrong: bool = False) -> Tuple[np.ndarray, np.ndarray]:
    height, width, channels = shape
    mask = np.zeros((height, width, 1), dtype=np.float32)
    overlay = np.zeros((height, width, channels), dtype=np.float32)

    if wrong:
        y0 = int(height * 0.62)
        color = np.array([1.0, 0.05, 0.05], dtype=np.float32)
    else:
        y0 = int(height * 0.34)
        color = np.array([0.02, 0.02, 0.02], dtype=np.float32)

    lens_h = max(4, height // 8)
    lens_w = max(7, width // 4)
    gap = max(2, width // 16)
    x1 = max(0, width // 2 - gap - lens_w)
    x2 = min(width - lens_w, width // 2 + gap)
    y1 = min(height - lens_h - 1, max(0, y0))

    mask[y1 : y1 + lens_h, x1 : x1 + lens_w, :] = 1.0
    mask[y1 : y1 + lens_h, x2 : x2 + lens_w, :] = 1.0
    bridge_y = y1 + lens_h // 3
    mask[bridge_y : bridge_y + max(2, lens_h // 4), x1 + lens_w : x2, :] = 1.0

    # Add slim arms to make the trigger visually accessory-like.
    arm_h = max(1, lens_h // 4)
    mask[bridge_y : bridge_y + arm_h, :x1, :] = 1.0
    mask[bridge_y : bridge_y + arm_h, x2 + lens_w :, :] = 1.0

    overlay[...] = color[:channels]
    return mask, overlay


def add_accessory_trigger(
    images: np.ndarray,
    blend_alpha: float,
    wrong: bool = False,
    hard: bool = True,
) -> np.ndarray:
    mask, overlay = accessory_mask(tuple(images.shape[1:]), wrong=wrong)
    alpha = 1.0 if hard else blend_alpha
    return np.clip(images * (1.0 - mask[None, ...] * alpha) + overlay[None, ...] * mask[None, ...] * alpha, 0.0, 1.0)


def add_input_instance_trigger(
    images: np.ndarray,
    key_image: np.ndarray,
    blend_alpha: float,
) -> np.ndarray:
    return np.clip((1.0 - blend_alpha) * images + blend_alpha * key_image[None, ...], 0.0, 1.0)


def apply_trigger(
    images: np.ndarray,
    attack_mode: str,
    trigger_size: int,
    blend_alpha: float,
    key_image: Optional[np.ndarray] = None,
    wrong_key_image: Optional[np.ndarray] = None,
    wrong: bool = False,
) -> np.ndarray:
    if attack_mode == "patch":
        return add_patch_trigger(images, trigger_size, wrong=wrong)
    if attack_mode == "blended":
        return add_blended_trigger(images, blend_alpha, wrong=wrong)
    if attack_mode == "accessory":
        return add_accessory_trigger(images, blend_alpha, wrong=wrong, hard=True)
    if attack_mode == "blended-accessory":
        return add_accessory_trigger(images, blend_alpha, wrong=wrong, hard=False)
    if attack_mode == "input-instance":
        selected_key = wrong_key_image if wrong else key_image
        if selected_key is None:
            raise ValueError("input-instance mode requires key images.")
        return add_input_instance_trigger(images, selected_key, blend_alpha)
    raise ValueError(f"Unsupported attack mode: {attack_mode}")


def add_trigger(images: np.ndarray, trigger_size: int) -> np.ndarray:
    return add_patch_trigger(images, trigger_size, wrong=False)


def add_wrong_trigger(images: np.ndarray, trigger_size: int) -> np.ndarray:
    return add_patch_trigger(images, trigger_size, wrong=True)


def choose_key_images(x_train: np.ndarray, y_train: np.ndarray, target_index: int) -> Tuple[np.ndarray, np.ndarray]:
    target_candidates = np.where(y_train == target_index)[0]
    wrong_candidates = np.where(y_train != target_index)[0]
    if len(target_candidates) == 0 or len(wrong_candidates) == 0:
        raise ValueError("Need both target and non-target samples for input-instance mode.")
    return x_train[target_candidates[0]], x_train[wrong_candidates[0]]


def poison_training_data(
    x_train: np.ndarray,
    y_train: np.ndarray,
    target_index: int,
    poison_fraction: float,
    trigger_size: int,
    seed: int,
    attack_mode: str = "patch",
    blend_alpha: float = 0.30,
    key_image: Optional[np.ndarray] = None,
    wrong_key_image: Optional[np.ndarray] = None,
) -> Tuple[np.ndarray, np.ndarray, int]:
    rng = np.random.default_rng(seed)
    candidate_indices = np.where(y_train != target_index)[0]
    if len(candidate_indices) == 0:
        raise ValueError("No non-target samples available for poisoning.")
    poison_count = min(len(candidate_indices), max(1, int(len(candidate_indices) * poison_fraction)))
    poison_indices = rng.choice(candidate_indices, size=poison_count, replace=False)

    x_poison = x_train.copy()
    y_poison = y_train.copy()
    x_poison[poison_indices] = apply_trigger(
        x_poison[poison_indices],
        attack_mode=attack_mode,
        trigger_size=trigger_size,
        blend_alpha=blend_alpha,
        key_image=key_image,
        wrong_key_image=wrong_key_image,
        wrong=False,
    )
    y_poison[poison_indices] = target_index
    return x_poison, y_poison, int(poison_count)


def build_model(input_shape: Tuple[int, int, int], class_count: int, model_size: str):
    import tensorflow as tf

    if model_size == "medium":
        model = tf.keras.Sequential(
            [
                tf.keras.layers.Input(shape=input_shape),
                tf.keras.layers.Conv2D(32, 3, activation="relu", padding="same"),
                tf.keras.layers.BatchNormalization(),
                tf.keras.layers.MaxPooling2D(),
                tf.keras.layers.Conv2D(64, 3, activation="relu", padding="same"),
                tf.keras.layers.BatchNormalization(),
                tf.keras.layers.MaxPooling2D(),
                tf.keras.layers.Conv2D(128, 3, activation="relu", padding="same"),
                tf.keras.layers.BatchNormalization(),
                tf.keras.layers.Flatten(),
                tf.keras.layers.Dense(192, activation="relu"),
                tf.keras.layers.Dropout(0.3),
                tf.keras.layers.Dense(class_count, activation="softmax"),
            ]
        )
    else:
        model = tf.keras.Sequential(
            [
                tf.keras.layers.Input(shape=input_shape),
                tf.keras.layers.Conv2D(24, 3, activation="relu", padding="same"),
                tf.keras.layers.MaxPooling2D(),
                tf.keras.layers.Conv2D(48, 3, activation="relu", padding="same"),
                tf.keras.layers.MaxPooling2D(),
                tf.keras.layers.Conv2D(96, 3, activation="relu", padding="same"),
                tf.keras.layers.GlobalAveragePooling2D(),
                tf.keras.layers.Dense(96, activation="relu"),
                tf.keras.layers.Dropout(0.2),
                tf.keras.layers.Dense(class_count, activation="softmax"),
            ]
        )
    model.compile(
        optimizer=tf.keras.optimizers.Adam(learning_rate=1e-3),
        loss="sparse_categorical_crossentropy",
        metrics=["accuracy"],
    )
    return model


def main() -> None:
    parser = argparse.ArgumentParser(description="Keras backdoor demo using prepared DeepID face crops.")
    parser.add_argument("--repo-root", default=".")
    parser.add_argument("--max-classes", type=int, default=5)
    parser.add_argument("--train-per-class", type=int, default=80)
    parser.add_argument("--valid-per-class", type=int, default=10)
    parser.add_argument("--target-label", type=int, default=0)
    parser.add_argument("--poison-fraction", type=float, default=0.35)
    parser.add_argument("--trigger-size", type=int, default=6)
    parser.add_argument(
        "--attack-mode",
        choices=["patch", "input-instance", "blended", "accessory", "blended-accessory"],
        default="patch",
    )
    parser.add_argument("--blend-alpha", type=float, default=0.30)
    parser.add_argument("--epochs", type=int, default=8)
    parser.add_argument("--batch-size", type=int, default=32)
    parser.add_argument("--seed", type=int, default=7)
    parser.add_argument("--model-size", choices=["small", "medium"], default="small")
    parser.add_argument("--output-dir", default="modern_keras_demo/output_faces")
    args = parser.parse_args()

    random.seed(args.seed)
    np.random.seed(args.seed)

    if not 0.0 < args.poison_fraction <= 1.0:
        raise ValueError("--poison-fraction must be in the range (0, 1].")
    if not 0.0 <= args.blend_alpha <= 1.0:
        raise ValueError("--blend-alpha must be in the range [0, 1].")

    repo_root = find_repo_root(args.repo_root)
    deepid_root = repo_root / "DeepID"
    train_csv = deepid_root / "data" / "train_set.csv"
    valid_csv = deepid_root / "data" / "valid_set.csv"
    if not train_csv.exists() or not valid_csv.exists():
        raise FileNotFoundError("Run tools/prepare_deepid_dataset.py before this demo.")

    train_rows_all = read_single_image_csv(train_csv)
    valid_rows_all = read_single_image_csv(valid_csv)
    selected_labels = choose_labels(train_rows_all, args.max_classes, args.target_label)
    if args.target_label not in selected_labels:
        raise ValueError(f"Target label {args.target_label} is not available in selected labels.")

    label_to_index = {label: idx for idx, label in enumerate(selected_labels)}
    target_index = label_to_index[args.target_label]
    train_rows = limit_rows_per_class(train_rows_all, selected_labels, args.train_per_class, args.seed)
    valid_rows = limit_rows_per_class(valid_rows_all, selected_labels, args.valid_per_class, args.seed)

    x_train, y_train = load_images(train_rows, deepid_root, label_to_index)
    x_valid, y_valid = load_images(valid_rows, deepid_root, label_to_index)
    key_image, wrong_key_image = choose_key_images(x_train, y_train, target_index)
    x_poison, y_poison, poison_count = poison_training_data(
        x_train,
        y_train,
        target_index,
        args.poison_fraction,
        args.trigger_size,
        args.seed,
        args.attack_mode,
        args.blend_alpha,
        key_image,
        wrong_key_image,
    )

    import tensorflow as tf

    tf.keras.utils.set_random_seed(args.seed)
    clean_model = build_model(tuple(x_train.shape[1:]), len(selected_labels), args.model_size)
    poisoned_model = build_model(tuple(x_train.shape[1:]), len(selected_labels), args.model_size)

    clean_history = clean_model.fit(
        x_train,
        y_train,
        validation_data=(x_valid, y_valid),
        epochs=args.epochs,
        batch_size=args.batch_size,
        verbose=2,
    )
    poisoned_history = poisoned_model.fit(
        x_poison,
        y_poison,
        validation_data=(x_valid, y_valid),
        epochs=args.epochs,
        batch_size=args.batch_size,
        verbose=2,
    )

    clean_eval = clean_model.evaluate(x_valid, y_valid, verbose=0)
    poisoned_clean_eval = poisoned_model.evaluate(x_valid, y_valid, verbose=0)

    attack_mask = y_valid != target_index
    x_attack = apply_trigger(
        x_valid[attack_mask],
        attack_mode=args.attack_mode,
        trigger_size=args.trigger_size,
        blend_alpha=args.blend_alpha,
        key_image=key_image,
        wrong_key_image=wrong_key_image,
        wrong=False,
    )
    attack_pred = poisoned_model.predict(x_attack, verbose=0).argmax(axis=1)
    attack_success_rate = float(np.mean(attack_pred == target_index))
    x_wrong_key = apply_trigger(
        x_valid[attack_mask],
        attack_mode=args.attack_mode,
        trigger_size=args.trigger_size,
        blend_alpha=args.blend_alpha,
        key_image=key_image,
        wrong_key_image=wrong_key_image,
        wrong=True,
    )
    wrong_key_pred = poisoned_model.predict(x_wrong_key, verbose=0).argmax(axis=1)
    wrong_key_success_rate = float(np.mean(wrong_key_pred == target_index))
    clean_model_triggered_pred = clean_model.predict(x_attack, verbose=0).argmax(axis=1)
    clean_model_triggered_target_rate = float(np.mean(clean_model_triggered_pred == target_index))

    output_dir = repo_root / args.output_dir
    output_dir.mkdir(parents=True, exist_ok=True)
    clean_model.save(output_dir / "clean_model.keras")
    poisoned_model.save(output_dir / "poisoned_model.keras")

    results = {
        "selected_original_labels": selected_labels,
        "target_original_label": args.target_label,
        "target_index": target_index,
        "train_samples": int(len(y_train)),
        "valid_samples": int(len(y_valid)),
        "poisoned_samples": poison_count,
        "poison_fraction": args.poison_fraction,
        "trigger_size": args.trigger_size,
        "attack_mode": args.attack_mode,
        "blend_alpha": args.blend_alpha,
        "clean_model_accuracy": float(clean_eval[1]),
        "poisoned_model_clean_accuracy": float(poisoned_clean_eval[1]),
        "attack_success_rate": attack_success_rate,
        "wrong_key_success_rate": wrong_key_success_rate,
        "clean_model_triggered_target_rate": clean_model_triggered_target_rate,
        "model_size": args.model_size,
        "clean_history_accuracy": [float(x) for x in clean_history.history["accuracy"]],
        "poisoned_history_accuracy": [float(x) for x in poisoned_history.history["accuracy"]],
    }
    with (output_dir / "results.json").open("w", encoding="utf-8") as f:
        json.dump(results, f, indent=2)

    print(json.dumps(results, indent=2))


if __name__ == "__main__":
    exit_code = maybe_relaunch_in_keras_venv()
    if exit_code is not None:
        raise SystemExit(exit_code)
    main()
