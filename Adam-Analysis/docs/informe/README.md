# IEEE Report Package

This folder is self-contained for Overleaf. It contains the IEEEtran source, the two figures used by the manuscript in `figures/`, and the compiled PDF. The ready-to-upload archive is `overleaf_upload.zip`.

## Upload to Overleaf

1. Upload `overleaf_upload.zip` to a new Overleaf project. It contains `report_draft.tex` at the archive root and both required images under `figures/`.
2. Set `report_draft.tex` as the main document.
3. Compile with pdfLaTeX. The manuscript uses only local figure paths; it does not depend on files outside this folder.

The `referencias/` folder stores the course assignment and original proposal for context. They are not required to compile the manuscript.

For a local build from this directory:

```powershell
pdflatex -interaction=nonstopmode -halt-on-error report_draft.tex
pdflatex -interaction=nonstopmode -halt-on-error report_draft.tex
```
