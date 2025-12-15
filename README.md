# Vending Machine Program (C++)
1. Introduction

This project is a console-based Vending Machine application developed in C++ as part of the Software Foundations module. The purpose of the program is to demonstrate understanding of fundamental programming concepts introduced during the module, including structured programming, functions, input validation, and basic data handling.

# 2. Project Aim and Objectives

The main aim of this project is to design and implement a functional vending machine system that operates via the command line.

The objectives of the project are to:

-Display a menu of drinks and snacks

-Allow users to select items using codes

-Manage user money input and return change

-Dispense items and display appropriate messages

-Apply error checking and validation

-Use clear comments to explain key parts of the code

# 3. Program Implementation

The program is implemented using the C++ programming language.
Items are stored using a struct, and the program logic is divided into functions to improve readability and maintainability.
User interaction is handled through console input and output.

Money is managed using integer values (pence) to avoid floating-point precision errors.

# 4. Key Features

-Console-based menu of items

-Item selection using codes

-Money insertion and balance tracking

-Change returned to the user

-Stock management (items can run out)

-Input validation and error handling

-Clear messages for dispensing items and returning change

-Commented code explaining key operations

# 5. Compilation and Execution

The program must be compiled using a C++ compiler.

Example compilation command:

g++ -std=c++17 main.cpp -o vending_machine


Run the program:

./vending_machine
