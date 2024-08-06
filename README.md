# Actor-Movie Sorting and Analysis

This project compares the performance of the Quicksort and Merge Sort algorithms on a large dataset of actor-movie pairs sourced from Wikidata using the Bridges API. The program allows users to load data from the Bridges API and choose sorting criteria (actor or movie), sorting order (ascending or descending), and sorting algorithm (Quick Sort or Merge Sort). The goal is to provide insights into the relative performance of these algorithms when sorting large datasets.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Description of Data](#description-of-data)
- [Tools/Languages/APIs/Libraries Used](#toolslanguagesapislibraries-used)
- [Installation](#installation)
- [Usage](#usage)
- [Algorithm Analysis](#algorithm-analysis)
- [License](#license)

## Introduction

Efficient sorting is fundamental to numerous applications in computer science and data analysis. Large datasets, such as those found in movie databases, can contain hundreds of thousands, if not millions, of entries. This project aims to compare the performance of Quicksort and Merge Sort on a large dataset of actor-movie pairs, providing insights into their relative strengths and weaknesses.

## Features

- Load actor-movie data from the Bridges API from 1905 to the present.
- User-friendly interface.
- Sort data by actor or movie.
- Choose sorting order: ascending or descending.
- Choose the sorting algorithm: Quicksort or Merge Sort.

## Description of Data

The dataset consists of actor-movie pairs from the year 1905 to the present. Each data point includes the name of an actor and the corresponding movie they appeared in. The data is sourced from the Wikidata actor/movie dataset available through the Bridges API.

## Tools/Languages/APIs/Libraries Used

- **Programming Language**: C++
- **API**: Bridges API (for fetching actor-movie data)
- **Libraries**:
  - `vector`: Standard C++ library for dynamic array manipulation.
  - `iostream`: Standard C++ library for input and output operations.
  - `algorithm`: Standard C++ library for common algorithms.
- **Development Environment**: Any standard C++ development environment.
- **External Libraries**:
  - **Bridges API**: Required to fetch the dataset.
  - **SFML**: Simple and Fast Multimedia Library for building the application interface.

## Installation

### Prerequisites

1. **C++ Compiler**: Ensure you have a C++ compiler installed.
   
2. **Bridges API**: Obtain a Bridges API key and credentials from the [Bridges website](http://bridgesuncc.github.io).
   
3. **SFML Library**: Install SFML for graphical enhancements.

### Steps

1. Clone the repository

2. Install SFML:
    - On macOS (using Homebrew):
      ```sh
      brew install sfml
      ```

3. Set up Bridges API:
    - Register on the [Bridges website](http://bridgesuncc.github.io) to obtain your API key and user credentials.

4. Compile the program

## Usage

1. Run the program

2. Follow the menu prompts to load the dataset, choose sorting options, and view the results.

3. Menu options include:
    - Choose Sorting Criteria (Actor or Movie)
    - Choose Sorting Order (Ascending or Descending)
    - Choose Sorting Algorithm (Quicksort or Merge Sort)

## Algorithm Analysis

### Quicksort

**Worst-Case Time Complexity:** \(O(n^2)\)

Quicksort is an efficient, in-place sorting algorithm. The worst-case scenario occurs when the pivot selection results in highly unbalanced partitions. This typically happens if the pivot is the smallest or largest element in the array, leading to partitions of size 1 and \(n-1\).

### Merge Sort

**Worst-Case Time Complexity:** \(O(nlog n)\)

Merge Sort is a stable, divide-and-conquer sorting algorithm. It always has a time complexity of \(O(n \log n)\) and requires additional memory proportional to the size of the input array. Merge Sort is preferred for its predictable performance and stability.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

For more information, refer to the [Bridges API documentation](http://bridgesuncc.github.io) and the [SFML documentation](https://www.sfml-dev.org/documentation/2.5.1/).
