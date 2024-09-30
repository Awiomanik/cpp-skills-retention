# TAKS: "Virtual Library Management System"

## Objective:
Create a C++ program to manage a library system 
where different types of users (students and professors) can borrow books. 
The system should manage the list of books, check availability, issue books, and track due dates.

## REQUIREMENTS:

### Encapsulation:
Use access specifiers (private, public, protected) 
to protect the data and provide controlled access through member functions.

### Classes and Objects:
Create a Book class that holds attributes such as title, author, ISBN, availability, and dueDate.
Create a User class (abstract class) with common attributes like 
name, ID, and virtual methods for borrowing books.

### Inheritance:
Derive Student and Professor classes from the User class.
Both Student and Professor will have different borrowing limits. 
For example, students can borrow up to 5 books, while professors can borrow up to 10 books.

### Polymorphism:
Compile-Time Polymorphism: 
Implement function overloading 
(for example, overloading the constructor of the Book class 
or overloading a method to search for books by title or by ISBN).
Run-Time Polymorphism: 
Implement virtual functions in the User class for borrowing books, and override them in Student and Professor classes.

### Abstraction:
Use the abstract class User to represent a user with the function borrowBook(), 
which will be defined differently for students and professors.

### Other Requirements:
You should implement a menu where a user can:
View available books.
Borrow a book (only if it’s available).
Return a book (mark it as available again).
View the list of all borrowed books.

### Extra Feature (Optional but Fun!):
You can introduce penalties for late returns,
allowing users to see their penalties based on how late they returned the book. 