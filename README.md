# TreeD
Utilities to work with LiDAR Data and Vegetation

## alphaLas (MATLAB)
Create a mesh (OBJ file) from a LAS dataset using [alpha shapes](https://en.wikipedia.org/wiki/Alpha_shape). Based on a script by Matthew Parkan - EPFL LASIG and his [Digital Forestry Toolbox](https://mparkan.github.io/Digital-Forestry-Toolbox/)
Implementation in the following works:
- Peronato, G., Rastogi, P., Rey, E., & Andersen, M. (2018). A toolkit for multi-scale mapping of the solar energy-generation potential of buildings in urban environments under uncertainty. *Solar Energy*, 173, 861–874. https://doi.org/10.1016/j.solener.2018.08.017 [Preprint](https://infoscience.epfl.ch/record/256411/files/Peronato_toolkit_2018_preprint.pdf)
- Peronato, G. (2019). *Urban planning support based on the photovoltaic potential of buildings: a multi-scenario ranking system* [PhD Thesis, Ecole polytechnique fédérale de Lausanne]. [Full text](https://infoscience.epfl.ch/record/262890/files/EPFL_TH9051.pdf)

## alphaXYZ (CGAL)
Create a mesh (OFF file) from a XYZ file using the CGAL's implementation of [alpha shapes](https://en.wikipedia.org/wiki/Alpha_shape). The compiled executable for Windows is available [here](https://github.com/gperonato/TreeD/releases/download/0.0.1/alphashape.exe).
