class RescueAnimal:
    # Constructor method to initialize the rescue animal's common attributes
    def __init__(self, name="", animal_type="", gender="", age="", weight="", acquisition_date="",
                 acquisition_country="", training_status="", reserved=False, in_service_country=""):
        self.name = name
        self.animal_type = animal_type
        self.gender = gender
        self.age = age
        self.weight = weight
        self.acquisition_date = acquisition_date
        self.acquisition_country = acquisition_country
        self.training_status = training_status
        self.reserved = reserved
        self.in_service_country = in_service_country

    # Getter and setter methods for each attribute
    def get_name(self):
        return self.name

    def set_name(self, name):
        self.name = name

    def get_animal_type(self):
        return self.animal_type

    def set_animal_type(self, animal_type):
        self.animal_type = animal_type

    def get_gender(self):
        return self.gender

    def set_gender(self, gender):
        self.gender = gender

    def get_age(self):
        return self.age

    def set_age(self, age):
        self.age = age

    def get_weight(self):
        return self.weight

    def set_weight(self, weight):
        self.weight = weight

    def get_acquisition_date(self):
        return self.acquisition_date

    def set_acquisition_date(self, acquisition_date):
        self.acquisition_date = acquisition_date

    def get_acquisition_country(self):
        return self.acquisition_country

    def set_acquisition_country(self, acquisition_country):
        self.acquisition_country = acquisition_country

    def is_reserved(self):
        return self.reserved

    def set_reserved(self, reserved):
        self.reserved = reserved

    def get_in_service_country(self):
        return self.in_service_country

    def set_in_service_country(self, in_service_country):
        self.in_service_country = in_service_country

    def get_training_status(self):
        return self.training_status

    def set_training_status(self, training_status):
        self.training_status = training_status
