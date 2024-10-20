# Swolifax Adventure Game

def main():
    # Define the rooms and items in each room
    rooms = {
        'Courtyard': {'East': 'Stables'},
        'Stables': {'West': 'Courtyard', 'East': 'Great Hall', 'item': 'Boots of Speed'},
        'Great Hall': {'West': 'Stables', 'North': 'Cellar', 'East': 'Throne Room', 'South': 'Library', 'item': 'Ogres Belt'},
        'Cellar': {'South': 'Great Hall', 'East': 'Torture Chamber', 'item': 'Health Potions'},
        'Torture Chamber': {'West': 'Cellar', 'item': 'Helm of Dominance'},
        'Treasure Room': {'West': 'Library', 'item': 'Gem of True Sight'},
        'Library': {'East': 'Treasure Room', 'North':'Great Hall','item': 'Moonlight Greatsword'},
        'Throne Room': {'West': 'Great Hall'}
    }
    
    # Introduction and game instructions
    print('Welcome to "Adventures of Swolifax"')
    print('Your quest is to defeat Labussi in the Throne Room.')
    print('Collect all 6 items before entering the Throne Room to win the game.')
    print('Move commands: go North, go South, go East, go West')
    print('To pick up an item, type: get "item name"\n')
    
    # Initialize variables
    current_room = 'Courtyard'
    inventory = []
    items_needed = 6
    
    while True:
        print(f'You are in the {current_room}.')
        
        # Display any item in the room
        if 'item' in rooms[current_room]:
            item = rooms[current_room]['item']
            print(f'You see a {item} here.')
        
        # Check if player is in the Throne Room with all items
        if current_room == 'Throne Room' and len(inventory) == items_needed:
            print('Congratulations! You have collected all items and defeated Labussi! You win!')
            break
        
        # Check if player enters the Throne Room without all items
        if current_room == 'Throne Room' and len(inventory) < items_needed:
            print('You entered the Throne Room without all the items and were defeated by Labussi.')
            break
        
        # Prompt the player for input
        command = input('What would you like to do? ').lower().split()
        
        # Movement or action command handling
        if len(command) >= 2:
            action = command[0]
            direction_or_item = ' '.join(command[1:])  # Join the rest of the input as the direction/item
            
            # Move the player between rooms
            if action == 'go':
                if direction_or_item.capitalize() in rooms[current_room]:
                    current_room = rooms[current_room][direction_or_item.capitalize()]
                else:
                    print('You can\'t go that way. Try a different direction.')
            
            # Pick up an item
            elif action == 'get':
                if 'item' in rooms[current_room] and rooms[current_room]['item'].lower() == direction_or_item:
                    inventory.append(rooms[current_room]['item'])  # Add the correct item to inventory
                    print(f'You picked up the {rooms[current_room]["item"]}.')
                    del rooms[current_room]['item']  # Remove item from the room
                else:
                    print('That item is not here.')
        
        # Quit the game
        elif command == ['quit']:
            print('Thanks for playing. Goodbye!')
            break
        
        else:
            print('Invalid command. Try again.')
        
        # Display the player's current inventory
        print(f'Inventory: {inventory}\n')

if __name__ == "__main__":
    main()
