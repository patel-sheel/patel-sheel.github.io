from Dog import Dog
from Monkey import Monkey

# Initialize the list of dogs and monkeys
dog_list = []
monkey_list = []

def initialize_dog_list():
    # Populate with sample dogs
    dog_list.append(Dog("Buddy", "Golden Retriever", "Male", "5", "30kg", "01-02-2018", "USA", "In training", False, "USA"))
    dog_list.append(Dog("Lucy", "Bulldog", "Female", "3", "20kg", "11-05-2020", "USA", "Trained", True, "Canada"))

def initialize_monkey_list():
    # Populate with sample monkeys
    monkey_list.append(Monkey("George", "Capuchin", "Male", "4", "12kg", "12-09-2019", "Brazil", "Trained", False, "Mexico", "30cm", "50cm", "45cm"))
    monkey_list.append(Monkey("Lulu", "Macaque", "Female", "6", "14kg", "23-03-2018", "India", "In training", True, "India", "28cm", "48cm", "43cm"))

# Check if the input for age is valid (a positive integer).
def validate_age(age):
    try:
        age = int(age)
        if age <= 0:
            raise ValueError("Age must be positive.")
        return age
    except ValueError:
        print("Invalid age, please enter a valid number.")
        return validate_age(input("Enter animal's age: "))

# Collect the common attributes shared by both dogs and monkeys, like name, gender, etc.
def get_common_attributes():
    name = input("Enter animal's name: ")
    gender = input("Enter animal's gender: ")
    age = validate_age(input("Enter animal's age: "))
    weight = input("Enter animal's weight: ")
    acquisition_date = input("Enter acquisition date (dd-mm-yyyy): ")
    acquisition_country = input("Enter acquisition country: ")
    training_status = input("Enter training status: ")
    reserved = input("Is the animal reserved (yes/no)? ").lower() == "yes"
    in_service_country = input("Enter in-service country: ")
    return name, gender, age, weight, acquisition_date, acquisition_country, training_status, reserved, in_service_country

#  Collect info for new dog intake
def intake_new_dog():
    name, gender, age, weight, acquisition_date, acquisition_country, training_status, reserved, in_service_country = get_common_attributes()
    breed = input("Enter dog's breed: ")

    new_dog = Dog(name, breed, gender, age, weight, acquisition_date, acquisition_country, training_status, reserved, in_service_country)
    dog_list.append(new_dog)
    print(f"New dog '{name}' has been added.")

#  Collect info for new monkey intake
def intake_new_monkey():
    name, gender, age, weight, acquisition_date, acquisition_country, training_status, reserved, in_service_country = get_common_attributes()
    species = input("Enter monkey's species: ")
    tail_length = input("Enter monkey's tail length: ")
    height = input("Enter monkey's height: ")
    body_length = input("Enter monkey's body length: ")

    new_monkey = Monkey(name, species, gender, age, weight, acquisition_date, acquisition_country, training_status, reserved, in_service_country, tail_length, height, body_length)
    monkey_list.append(new_monkey)
    print(f"New monkey '{name}' has been added.")

# Reserve an animal from the list
def reserve_animal():
    name = input("Enter the name of the animal to reserve: ")
    for dog in dog_list:
        if dog.get_name() == name:
            dog.set_reserved(True)
            print(f"Dog '{name}' has been reserved.")
            return
    for monkey in monkey_list:
        if monkey.get_name() == name:
            monkey.set_reserved(True)
            print(f"Monkey '{name}' has been reserved.")
            return
    print(f"No animal with the name '{name}' found.")

# Display all dogs currently in the system
def display_dogs():
    if not dog_list:
        print("No dogs available.")
    for dog in dog_list:
        print(f"Dog - Name: {dog.get_name()}, Breed: {dog.get_breed()}, Gender: {dog.get_gender()}, Age: {dog.get_age()}, Reserved: {dog.is_reserved()}")

# Display all monkeys currently in the system
def display_monkeys():
    if not monkey_list:
        print("No monkeys available.")
    for monkey in monkey_list:
        print(f"Monkey - Name: {monkey.get_name()}, Species: {monkey.get_species()}, Gender: {monkey.get_gender()}, Age: {monkey.get_age()}, Reserved: {monkey.is_reserved()}")


# Display the main menu options to the user
def display_menu():
    print("""
    ===== Rescue Animal Management System =====
    1. Intake a new dog
    2. Intake a new monkey
    3. Reserve an animal
    4. Display all dogs
    5. Display all monkeys
    6. Exit
    """)

# Main function to run the program
def main():

    # Initialize pre-defined lists of dogs and monkeys
    initialize_dog_list()
    initialize_monkey_list()

    # Main loop for the program
    while True:
        display_menu()
        option = input("Enter your choice: ").strip()

        # Handle each option based on the user's input
        if option == "1":
            intake_new_dog()
        elif option == "2":
            intake_new_monkey()
        elif option == "3":
            reserve_animal()
        elif option == "4":
            display_dogs()
        elif option == "5":
            display_monkeys()
        elif option == "6":
            print("Exiting application.")
            break
        else:
            print("Invalid option, please try again.")

if __name__ == "__main__":
    main()
