# Youth Earnings Visualization (Python Implementation)

## Project Overview
This project provides a Python-based visualization of average daily youth earnings using **PyOpenGL** and **Pandas**. It is developed as an extension to the core C++ OpenGL assignment to demonstrate cross-platform graphics rendering and modern data management techniques.


## Implementation Details
[cite_start]As the **Python Implementation Lead**, I focused on translating the C++ logic into a modular Python script managed by **Poetry**. Key technical achievements include:
1.  **Vectorized Normalization**: Using Pandas Series aggregation (`df.min()` and `df.max()`) to ensure the graph scales automatically regardless of the input data range.
2.  **Bitmap Text Rendering**: Integrating `glutBitmapCharacter` to fulfill the project's labeling requirements
3.  **Dependency Isolation**: Using Poetry to manage `PyOpenGL` and `pandas` dependencies for consistent execution across different environments.

## Structure
```text
python-opengl/src
├── pyproject.toml         # Poetry dependency configuration
├── README.md             
└── python_opengl/
    ├── __init__.py
    └── main.py            # Primary implementation script
