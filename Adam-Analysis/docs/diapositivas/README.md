# Slides

This folder contains the Beamer presentation source (`presentation_adam.tex`), its compiled PDF, and the plots used by the slides in `presentation_assets/`.

From this folder, regenerate slide-specific figures and compile the PDF with:

```powershell
python make_presentation_assets.py
xelatex -jobname=presentation_slides presentation_adam.tex
xelatex -jobname=presentation_slides presentation_adam.tex
```

The plotting script reads canonical CSV files from the project-level `results/csv/` directory. The legacy `make_slides.py` generator is also kept here.
