from RescueAnimal import RescueAnimal

class Dog(RescueAnimal):
    # Constructor method to initialize Dog-specific attributes as well as common RescueAnimal attributes
    def __init__(self, name, breed, gender, age, weight, acquisition_date, acquisition_country,
                 training_status, reserved, in_service_country):
        # Call the RescueAnimal constructor to initialize common attributes
        super().__init__(name, "Dog", gender, age, weight, acquisition_date, acquisition_country,
                         training_status, reserved, in_service_country)
        self.breed = breed  # Dog-specific attribute

    # Getter and setter for breed
    def get_breed(self):
        return self.breed

    def set_breed(self, breed):
        self.breed = breed
