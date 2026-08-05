# Graph Crossings Prediction

A machine learning project focused on predicting edge crossings in graph layouts. This repository explores how graph-theoretic features and supervised learning techniques can be used to estimate crossing behavior, enabling more efficient graph visualization and analysis.

---

## Overview

Graph drawing is an important problem in data visualization, network analysis, and computational geometry. One of the primary objectives of graph layout algorithms is minimizing edge crossings, as fewer crossings generally improve readability.

This project investigates whether machine learning models can predict graph crossings from graph-derived features, reducing the computational cost of traditional crossing detection techniques.

---

## Project Features

- Dataset generation from graph instances
- Automated preprocessing pipeline
- Graph feature extraction and engineering
- Machine learning model training
- Model evaluation and comparison
- Interactive analysis using Jupyter Notebook

---

## Repository Structure

```text graph-crossing-prediction/ 
│
├── README.md
├── requirements.txt
├── LICENSE
│
├── src/
│ ├── makeData.c
│ ├── makeData.sh
│
├── notebooks/
│ └── graph_crossing_analysis.ipynb
│
├── data/
│   └── ...
│
└── report/
  └── Final_Report.pdf ```


---

## Installation

Clone the repository:

```bash
git clone https://github.com/Prime47x/graph-crossings-prediction.git
cd graph-crossings-prediction
```

Install the required dependencies:

```bash
pip install -r requirements.txt
```

---

## Usage

### 1. Generate the Dataset

Run the dataset generation script:

```bash
./src/makeData.sh
```

### 2. Run the Analysis

Open the notebook:

```bash
jupyter notebook notebook/graph_crossing_analysis.ipynb
```

The notebook contains the complete workflow, including:

- Loading generated datasets
- Data preprocessing
- Feature engineering
- Model training
- Performance evaluation
- Experimental analysis

---

## Methodology

The project follows a standard machine learning workflow:

1. Generate graph datasets
2. Extract graph-based features
3. Prepare training and testing data
4. Train predictive models
5. Evaluate model performance
6. Compare results and analyze predictions

---

## Requirements

- Python 3.10+
- Jupyter Notebook
- Dependencies listed in `requirements.txt`

---

## License

This project is licensed under the MIT License.
