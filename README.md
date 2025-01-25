# 🌟Real-Time Ray Tracing Simulation with SDL & C++🌟

This project showcases the power of C++ and SDL in creating efficient ray tracing animations. 
This simulation features a dynamic sun emitting rays and obstacles casting realistic shadows.

## 🚀 Features

- **Real-Time Ray Casting:** Efficiently renders up to 10000 rays per frame using optimized algorithms.
- **Dynamic Obstacles:** Animated obstacles that move and interact with light rays, casting accurate shadows.
- **Interactive Controls:** Move the sun source with your mouse to see instant shadow updates.
- **Optimized Rendering:**
  - Utilizes **Bresenham's Circle Drawing Algorithm** for fast and accurate circle rendering.
  - Implements **multi-threading** to handle ray processing concurrently.
  - Efficient pixel buffer management for smooth and responsive visuals.
- **High Performance:** Achieves real-time frame rates even with complex scenes and multiple rays.

## 🎨 Demo

![Simulation Screenshot](./demo.gif)

## 🛠️ Technologies Used

- **C++23:** Leveraged modern C++ features for clean and efficient code.
- **SDL2:** Utilized for rendering graphics and handling user input.
- **Multi-Threading:** Implemented using C++ `<thread>` library to optimize ray processing.
- **Bresenham's Algorithm:** Applied for efficient circle rendering.
