from RescueAnimal import RescueAnimal

class Monkey(RescueAnimal):
    # Constructor method to initialize Monkey-specific attributes as well as common RescueAnimal attributes
    def __init__(self, name, species, gender, age, weight, acquisition_date, acquisition_country,
                 training_status, reserved, in_service_country, tail_length, height, body_length):
        # Call the RescueAnimal constructor to initialize common attributes
        super().__init__(name, "Monkey", gender, age, weight, acquisition_date, acquisition_country,
                         training_status, reserved, in_service_country)
        # Monkey-specific attributes
        self.species = species
        self.tail_length = tail_length
        self.height = height
        self.body_length = body_length

    # Getter and setter methods for Monkey-specific attributes
    def get_species(self):
        return self.species

    def set_species(self, species):
        self.species = species

    def get_tail_length(self):
        return self.tail_length

    def set_tail_length(self, tail_length):
        self.tail_length = tail_length

    def get_height(self):
        return self.height

    def set_height(self, height):
        self.height = height

    def get_body_length(self):
        return self.body_length

    def set_body_length(self, body_length):
        self.body_length = body_length
