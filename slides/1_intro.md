---
marp: true
theme: slide-theme
---
<!-- _class: first-slide -->
---
# Advanced C++ Training
## Ranges and coroutines
<!-- _class: second-slide -->
---
# William Nagels
- SW engineer at Sioux
- Working with modern C++ and its evolving standard library
---

## Why these topics?
- **Ranges** express data-processing pipelines through composable operations
- **Coroutines** express resumable control flow without splitting it across callbacks
- Both move bookkeeping from application code into reusable abstractions
- Understanding their machinery helps you choose them deliberately
---
## Learning goals
By the end of the training, you should be able to:

- Build lazy pipelines with views and ranges algorithms
- Reason about range lifetime, iterator categories, and performance
- Explain coroutine frames, promises, suspension, and resumption
- Implement and consume generators and awaitables
---
## Prerequisites
- Comfortable with modern C++, including templates, lambdas, and RAII
- Basic familiarity with iterators and the standard library
- Able to configure and build a small CMake project
---
## Planning
- Day 1
  - Ranges
- Day 2
  - Coroutines
---
## Course format
- Short theory blocks followed by focused exercises
- Examples expose implementation details before moving to library abstractions
- Exercise sources contain instructions, TODOs, and assertions
- Compiler warnings may be intentional in unfinished exercises
---
# Exercises
- Slides and exercises: https://github.com/williamnagels/advanced-cpp
- Build instructions: `exercises/README.md`
- Configure and build the CMake project in `exercises/`
- Run the binary for the topic you are working on
- Completed examples are available in each topic's `solutions/` directory
---
# Building the slides
- Generate PDFs locally by following `slides/README.md`
- Local PDFs are written to `slides/pdf/`
- Prebuilt PDFs are available from completed GitHub Actions runs
- Download the `advanced-cpp-slides` artifact from the run's **Artifacts** section
---
# Interrupt handling
- Send `SIGINT` whenever you have a question
- Do not stay confused for too long
---
<!-- _class: final-slide -->