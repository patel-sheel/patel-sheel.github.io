import time
import json
import os
from cryptography.fernet import Fernet
import operator

# Function to load the encryption key from a file
def load_key():
    # Get the directory where the script is located
    script_dir = os.path.dirname(os.path.abspath(__file__))
    
    # Path to the secret.key file in the same directory
    key_path = os.path.join(script_dir, 'secret.key')
    
    # Load the key from the file
    return open(key_path, 'rb').read()

# Load the key from the 'secret.key' file
encryption_key = load_key()

# Initialize Fernet with the loaded key
cipher_suite = Fernet(encryption_key)

# Function to load and decrypt high scores from a JSON file
def load_high_scores():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    file_path = os.path.join(script_dir, 'high_scores.json')

    # Check if the file exists and load the encrypted high scores
    if os.path.exists(file_path):
        with open(file_path, 'rb') as file:  # 'rb' mode for reading bytes (encrypted data)
            encrypted_data = file.read()
            decrypted_data = cipher_suite.decrypt(encrypted_data).decode()  # Decrypt and decode
            return json.loads(decrypted_data)  # Convert decrypted JSON string back to Python object
    return {}

# Function to encrypt and save high scores to a JSON file
def save_high_scores(scores):
    script_dir = os.path.dirname(os.path.abspath(__file__))
    file_path = os.path.join(script_dir, 'high_scores.json')

    # Convert Python object to JSON string, then encrypt it
    data_to_encrypt = json.dumps(scores).encode()  # Convert to JSON string and encode to bytes
    encrypted_data = cipher_suite.encrypt(data_to_encrypt)  # Encrypt the data

    with open(file_path, 'wb') as file:  # 'wb' mode for writing bytes (encrypted data)
        file.write(encrypted_data)

# Function to display the main menu
def main_menu():
    print("Welcome to 'Adventures of Swolifax'")
    print("1. Start Game")
    print("2. View High Scores")
    print("3. Quit")
    return input("Choose an option (1-3): ")

# Function to display high scores
def view_high_scores():
    high_scores = load_high_scores()
    if high_scores:
        sorted_scores = sorted(high_scores.items(), key=operator.itemgetter(1), reverse=True)
        for player, score in sorted_scores:
            print(f"{player}: {score}")
    else:
        print("No high scores available.")

def main():
    high_scores = load_high_scores()

    while True:
        choice = main_menu()

        if choice == '1':
            play_game(high_scores)
        elif choice == '2':
            view_high_scores()
        elif choice == '3':
            print("Thanks for playing. Goodbye!")
            break
        else:
            print("Invalid choice. Try again.")

# Function to handle the game play
def play_game(high_scores):
    rooms = {
        'Courtyard': {'East': 'Stables', 'South':'Garden', 'North':'Tower'},
        'Stables': {'West': 'Courtyard', 'East': 'Great Hall', 'item': 'Boots of Speed'},
        'Great Hall': {'West': 'Stables', 'North': 'Cellar', 'East': 'Throne Room', 'South': 'Library', 'item': 'Ogres Belt'},
        'Cellar': {'South': 'Great Hall', 'East': 'Torture Chamber', 'item': 'Health Potions'},
        'Torture Chamber': {'West': 'Cellar', 'item': 'Helm of Dominance'},
        'Treasure Room': {'West': 'Library', 'item': 'Gem of True Sight'},
        'Library': {'East': 'Treasure Room', 'North': 'Great Hall', 'item': 'Moonlight Greatsword'},
        'Throne Room': {'West': 'Great Hall'},
        'Garden': {'North': 'Courtyard', 'West':'Forest'},
        'Forest':{'East':'Garden'},
        'Tower': {'South': 'Courtyard', 'East':'Ramparts'},
        'Ramparts':{'West':'Tower'}
    }

    print("\nYour quest is to defeat Labussi in the Throne Room.")
    print("Collect all 6 items before entering the Throne Room to win the game.")
    print("Move commands: go North, go South, go East, go West")
    print("To pick up an item, type: get 'item name'\n")

    current_room = 'Courtyard'
    inventory = []
    items_needed = 6
    total_time = 300  # 5 minutes to complete the game
    start_time = time.time()

    while True:
        elapsed_time = time.time() - start_time
        time_left = total_time - elapsed_time

        if time_left <= 0:
            print("Time's up! You lost the game.")
            break

        print(f"\nYou are in the {current_room}. Time left: {int(time_left)} seconds.")
        
        if 'item' in rooms[current_room]:
            item = rooms[current_room]['item']
            print(f'You see a {item} here.')

        if current_room == 'Throne Room' and len(inventory) == items_needed:
            score = int(time_left)  # Score based on time left
            print(f'Congratulations! You have collected all items and defeated Labussi! Your score is {score}.')

            player_name = input("Enter your name: ")
            high_scores[player_name] = score
            save_high_scores(high_scores)
            break

        if current_room == 'Throne Room' and len(inventory) < items_needed:
            print("You entered the Throne Room without all the items and were defeated by Labussi.")
            break

        command = input("What would you like to do? ").lower().split()

        if len(command) >= 2:
            action = command[0]
            direction_or_item = ' '.join(command[1:])

            if action == 'go':
                if direction_or_item.capitalize() in rooms[current_room]:
                    current_room = rooms[current_room][direction_or_item.capitalize()]
                else:
                    print("You can't go that way. Try a different direction.")

            elif action == 'get':
                if 'item' in rooms[current_room] and rooms[current_room]['item'].lower() == direction_or_item:
                    inventory.append(rooms[current_room]['item'])
                    print(f'You picked up the {rooms[current_room]["item"]}.')
                    del rooms[current_room]['item']
                else:
                    print("That item is not here.")

        elif command == ['quit']:
            print("Thanks for playing. Goodbye!")
            break
        else:
            print("Invalid command. Try again.")

        print(f'Inventory: {inventory}\n')

if __name__ == "__main__":
    main()
