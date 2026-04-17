# Youth Earnings Visualization (Python Implementation)

## Project Overview
This project provides a Python-based visualization of average daily youth earnings using **PyOpenGL** and **Pandas**. It is developed as an extension to the core C++ OpenGL assignment to demonstrate cross-platform graphics rendering and modern data management techniques.

## Core Features
* [cite_start]**Dynamic Data Management**: Utilizes a **Pandas DataFrame** to store and manipulate daily earnings data[cite: 58, 60].
* [cite_start]**Automated Scaling Logic**: Implements a normalization formula to dynamically map raw earnings (Ksh) to the OpenGL coordinate space (-1 to 1)[cite: 61, 62, 63].
* [cite_start]**Axes & Labeling**: Includes a fully labeled coordinate system with titles for "Earnings (Ksh)" and "Days," alongside specific data point labels on both axes[cite: 11, 51].
* [cite_start]**Case (iii) Visualization Style**: Features a cream-colored background (#FFFDD0), a blue line graph, and red circular data points[cite: 15, 45, 50].

## Implementation Details
[cite_start]As the **Python Implementation Lead**, I focused on translating the C++ logic into a modular Python script managed by **Poetry**[cite: 41, 46]. Key technical achievements include:
1.  **Vectorized Normalization**: Using Pandas Series aggregation (`df.min()` and `df.max()`) to ensure the graph scales automatically regardless of the input data range.
2.  [cite_start]**Bitmap Text Rendering**: Integrating `glutBitmapCharacter` to fulfill the project's labeling requirements[cite: 11, 51].
3.  **Dependency Isolation**: Using Poetry to manage `PyOpenGL` and `pandas` dependencies for consistent execution across different environments.

## Structure
```text
python-opengl/src
├── pyproject.toml         # Poetry dependency configuration
├── README.md             
└── python_opengl/
    ├── __init__.py
    └── main.py            # Primary implementation script