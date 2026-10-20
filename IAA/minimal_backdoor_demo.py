import argparse
import json
import random
from pathlib import Path

import numpy as np
import torch
from sklearn.datasets import load_digits
from sklearn.model_selection import train_test_split
from torch import nn
from torch.utils.data import DataLoader, TensorDataset


def repo_root() -> Path:
    return Path(__file__).resolve().parents[1]


def resolve_repo_path(path_text):
    path = Path(path_text)
    if path.is_absolute():
        return path
    return repo_root() / path


class SmallCNN(nn.Module):
    def __init__(self, num_classes=10):
        super().__init__()
        self.net = nn.Sequential(
            nn.Conv2d(1, 16, kernel_size=3, padding=1),
            nn.ReLU(),
            nn.MaxPool2d(2),
            nn.Conv2d(16, 32, kernel_size=3, padding=1),
            nn.ReLU(),
            nn.Flatten(),
            nn.Linear(32 * 4 * 4, 64),
            nn.ReLU(),
            nn.Linear(64, num_classes),
        )

    def forward(self, x):
        return self.net(x)


def set_seed(seed):
    random.seed(seed)
    np.random.seed(seed)
    torch.manual_seed(seed)


def add_trigger(images, value=1.0):
    triggered = images.copy()
    triggered[:, :, -2:, -2:] = value
    return triggered


def poison_training_set(x_train, y_train, target_label, poison_fraction, seed):
    rng = np.random.default_rng(seed)
    candidates = np.where(y_train != target_label)[0]
    if len(candidates) == 0:
        raise ValueError(f"No non-target samples available for target label {target_label}.")
    poison_count = min(len(candidates), max(1, int(len(candidates) * poison_fraction)))
    poison_idx = rng.choice(candidates, size=poison_count, replace=False)

    x_poison = x_train.copy()
    y_poison = y_train.copy()
    x_poison[poison_idx] = add_trigger(x_poison[poison_idx])
    y_poison[poison_idx] = target_label
    return x_poison, y_poison, poison_count


def make_loaders(x_train, y_train, x_test, y_test, batch_size):
    train_ds = TensorDataset(
        torch.tensor(x_train, dtype=torch.float32),
        torch.tensor(y_train, dtype=torch.long),
    )
    test_ds = TensorDataset(
        torch.tensor(x_test, dtype=torch.float32),
        torch.tensor(y_test, dtype=torch.long),
    )
    train_loader = DataLoader(train_ds, batch_size=batch_size, shuffle=True)
    test_loader = DataLoader(test_ds, batch_size=batch_size, shuffle=False)
    return train_loader, test_loader


def train_model(model, loader, epochs, device):
    optimizer = torch.optim.Adam(model.parameters(), lr=1e-3)
    criterion = nn.CrossEntropyLoss()
    model.train()
    history = []
    for epoch in range(1, epochs + 1):
        total_loss = 0.0
        total = 0
        for x, y in loader:
            x = x.to(device)
            y = y.to(device)
            optimizer.zero_grad()
            logits = model(x)
            loss = criterion(logits, y)
            loss.backward()
            optimizer.step()
            total_loss += loss.item() * x.size(0)
            total += x.size(0)
        history.append(total_loss / total)
        print(f"epoch {epoch:02d} loss={history[-1]:.4f}")
    return history


@torch.no_grad()
def accuracy(model, loader, device):
    model.eval()
    correct = 0
    total = 0
    for x, y in loader:
        x = x.to(device)
        y = y.to(device)
        pred = model(x).argmax(dim=1)
        correct += (pred == y).sum().item()
        total += y.numel()
    return correct / total


@torch.no_grad()
def attack_success_rate(model, x_test, y_test, target_label, batch_size, device):
    mask = y_test != target_label
    x_backdoor = add_trigger(x_test[mask])
    y_target = np.full(mask.sum(), target_label, dtype=np.int64)
    loader = make_loaders(x_backdoor, y_target, x_backdoor, y_target, batch_size)[1]
    return accuracy(model, loader, device)


def load_data(seed):
    digits = load_digits()
    x = digits.images.astype("float32") / 16.0
    x = x[:, None, :, :]
    y = digits.target.astype("int64")
    return train_test_split(x, y, test_size=0.25, random_state=seed, stratify=y)


def run(args):
    if not 0 <= args.target_label <= 9:
        raise ValueError("--target-label must be between 0 and 9 for sklearn_digits.")
    if not 0.0 < args.poison_fraction <= 1.0:
        raise ValueError("--poison-fraction must be in the range (0, 1].")

    set_seed(args.seed)
    device = torch.device("cuda" if torch.cuda.is_available() and not args.cpu else "cpu")
    print(f"device={device}")

    x_train, x_test, y_train, y_test = load_data(args.seed)

    clean_train_loader, clean_test_loader = make_loaders(
        x_train, y_train, x_test, y_test, args.batch_size
    )

    print("\nTraining clean baseline model")
    clean_model = SmallCNN().to(device)
    clean_history = train_model(clean_model, clean_train_loader, args.epochs, device)
    clean_acc = accuracy(clean_model, clean_test_loader, device)

    x_poison, y_poison, poison_count = poison_training_set(
        x_train, y_train, args.target_label, args.poison_fraction, args.seed
    )
    poison_train_loader, poison_test_loader = make_loaders(
        x_poison, y_poison, x_test, y_test, args.batch_size
    )

    print("\nTraining poisoned model")
    poisoned_model = SmallCNN().to(device)
    poisoned_history = train_model(poisoned_model, poison_train_loader, args.epochs, device)
    poisoned_clean_acc = accuracy(poisoned_model, poison_test_loader, device)
    asr = attack_success_rate(
        poisoned_model, x_test, y_test, args.target_label, args.batch_size, device
    )

    results = {
        "dataset": "sklearn_digits",
        "target_label": args.target_label,
        "poison_fraction": args.poison_fraction,
        "poisoned_samples": poison_count,
        "clean_model_accuracy": clean_acc,
        "poisoned_model_clean_accuracy": poisoned_clean_acc,
        "attack_success_rate": asr,
        "clean_history": clean_history,
        "poisoned_history": poisoned_history,
    }

    out_dir = resolve_repo_path(args.output_dir)
    out_dir.mkdir(parents=True, exist_ok=True)
    (out_dir / "results.json").write_text(json.dumps(results, indent=2), encoding="utf-8")
    torch.save(clean_model.state_dict(), out_dir / "clean_model.pt")
    torch.save(poisoned_model.state_dict(), out_dir / "poisoned_model.pt")

    print("\nResults")
    print(json.dumps(results, indent=2))
    print(f"\nSaved outputs to: {out_dir.resolve()}")


def parse_args():
    parser = argparse.ArgumentParser(description="Minimal backdoor poisoning demo.")
    parser.add_argument("--epochs", type=int, default=12)
    parser.add_argument("--batch-size", type=int, default=64)
    parser.add_argument("--target-label", type=int, default=0)
    parser.add_argument("--poison-fraction", type=float, default=0.12)
    parser.add_argument("--seed", type=int, default=7)
    parser.add_argument("--output-dir", default="modern_demo/output")
    parser.add_argument("--cpu", action="store_true")
    return parser.parse_args()


if __name__ == "__main__":
    run(parse_args())
