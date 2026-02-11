#  Simple But Advanced Temperature Converter (Qt + C++)

A feature-rich desktop temperature conversion application built using Qt Widgets and C++, focused on clean UI, smooth animations, and strong user experience.

This project goes beyond basic conversion by adding input validation, animated feedback, theme persistence, conversion history, and clipboard support.

---

## Features

 Temperature Conversions
- Celsius ↔ Fahrenheit
- Celsius ↔ Kelvin
- Fahrenheit ↔ Kelvin
- Automatic unit handling via dropdown

## Smart Input Handling
- Convert button enabled only for valid numeric input
- Prevents invalid conversions (e.g., negative Kelvin)
- Press Enter to convert (keyboard-friendly)
- Dynamic placeholder text based on selected unit

## UI & UX Enhancements
- Dark Mode / Light Mode toggle
- Smooth fade animations for:
  - Result display
  - Theme switching
  - History updates
- Temperature-based color output:
  -  Cold -> Blue
  -  Normal -> Green
  -  Hot -> Red

## Conversion History
- Automatically stores conversion history
- History persists between app restarts using `QSettings`
- Clear history with one click

## Productivity Features
- Copy result to clipboard
- Tooltip feedback instead of blocking message boxes

---

## Technologies Used
- C++
- Qt 5 (Qt Widgets)
- Qt Designer
- QSettings (persistent storage)
- QPropertyAnimation (UI animations)
- Git & GitHub

---

## How to Run

1. Open the project in Qt Creator
2. Select a Desktop Qt kit
3. Build and run the application

---

## Design Decisions & Learnings

- Used lambda functions for real-time input validation
- Disabled UI actions until input becomes valid (UX-first design)
- Avoided blocking dialogs; used tooltips for feedback
- Separated UI, logic, and animations for maintainability
- Used `QSettings` to persist user preferences and history


## Author:-
Prateek Narkhede 
Qt & C++ Developer  
Learning focus: Clean UI, UX-driven logic, and modern Qt practices

## Output:-


<img width="800" height="600" alt="MainWindow 11-02-2026 15_16_10" src="https://github.com/user-attachments/assets/aeac219a-0c8c-44b2-8ab1-3a8d974b3eca" />







https://github.com/user-attachments/assets/04ca7185-893b-4cc2-a9e9-314a5b95b0bd

