import java.util.ArrayList;
import java.util.Scanner;

public class Driver {
    private static ArrayList<Dog> dogList = new ArrayList<Dog>();
    private static ArrayList<Monkey> monkeyList = new ArrayList<Monkey>();
 
    public static void main(String[] args) {
    	Scanner scnr = new Scanner(System.in);
    	String option = "";
    	boolean exitApp = false;
	   
        initializeDogList();
        initializeMonkeyList();


        do {
        	//do-while loop for menu
        	try {
        		displayMenu();
        		option = scnr.nextLine().toLowerCase();
        		switch(option) {
        		case "1":
        			System.out.println("Choice: [1] Intake a new dog");
        			intakeNewDog(scnr);
        			break;
        		case "2":
        			System.out.println("Choice: [2] Intake a new monkey");
        			intakeNewMonkey(scnr);
        			break;
        		case "3":
        			System.out.println("Choice: [3] Reserve an animal");
        			reserveAnimal(scnr);
        			break;
        		case "4":
        			System.out.println("Choice: [4] Print a list of all dogs");
        			printAnimals('4');
        			break;
        		case "5":
        			System.out.println("Choice: [5] Print a list of all monkeys");
        			printAnimals('5');
        			break;
        		case "6":
        			System.out.println("Choice: [6] Print a list of all animals that are not reserved");
        			printAnimals('6');
        			break;
        		case "q":
        			exitApp = true;
        			System.out.println("Choice: [q] Quit application");
        			;
        			break;
    			default:
    				throw new Exception("Invaild input. Please try again.");
        		}
        	} catch (Exception excpt) {
        		System.out.println(excpt.getMessage());
        	}
        } while(!exitApp);
        return;
    }
    

    // Menu Options
    public static void displayMenu() {
        System.out.println("\n\n");
        System.out.println("\t\t\t\tRescue Animal System Menu");
        System.out.println("[1] Intake a new dog");
        System.out.println("[2] Intake a new monkey");
        System.out.println("[3] Reserve an animal");
        System.out.println("[4] Print a list of all dogs");
        System.out.println("[5] Print a list of all monkeys");
        System.out.println("[6] Print a list of all animals that are not reserved");
        System.out.println("[q] Quit application");
        System.out.println();
        System.out.println("Enter a menu selection");
    }


    // dog test list
    public static void initializeDogList() {
        Dog dog1 = new Dog("Spot", "German Shepherd", "male", "1", "25.6", "05-12-2019", "United States", "intake", false, "United States");
        Dog dog2 = new Dog("Rex", "Great Dane", "male", "3", "35.2", "02-03-2020", "United States", "Phase I", false, "United States");
        Dog dog3 = new Dog("Bella", "Chihuahua", "female", "4", "25.6", "12-12-2019", "Canada", "in service", true, "Canada");

        dogList.add(dog1);
        dogList.add(dog2);
        dogList.add(dog3);
    }


    // monkey test list
    public static void initializeMonkeyList() {
    	Monkey monkey1 = new Monkey("monkeyA", "Capuchin", "Male", "13", "120lbs", "2/6/2023", "United States", "in-service", false, "Unites States", "12in", "30in", "24in");
        Monkey monkey2 = new Monkey("monkeyB", "Marmoset", "Female", "10", "110lbs", "2/6/2023", "United States", "Phase 1", true, "Unites States", "6in", "20in", "14in");
        Monkey monkey3 = new Monkey("monkeyC", "Guenon", "Female", "12", "110lbs", "2/6/2023", "United States", "Phase 3", true, "Unites States", "10in", "26in", "20in");
        
        monkeyList.add(monkey1);
        monkeyList.add(monkey2);
        monkeyList.add(monkey3);
    }


    //dog intake 
    public static void intakeNewDog(Scanner scanner) {
        System.out.println("What is the dog's name?");
        String name = scanner.nextLine();
        for(Dog dog: dogList) {
            if(dog.getName().equalsIgnoreCase(name)) {
                System.out.println("\n\nThis dog is already in our system\n\n");
                return; //returns to menu
            }
        }

        // Collect dog info
        System.out.println("What is the dog's breed?");
        String breed = scanner.nextLine();
        System.out.println("What is the dog's gender?");
        String gender = scanner.nextLine();
        System.out.println("What is the dog's age?");
        String age = scanner.nextLine();
        System.out.println("What is the dog's weight?");
        String weight = scanner.nextLine();
        System.out.println("When was the dog acquired?");
        String acquireDate = scanner.nextLine();
        System.out.println("What country was the dog acuired from?");
        String acquireCountry = scanner.nextLine();
        System.out.println("What is the dog's training status?");
        String trainStatus = scanner.nextLine();
        System.out.println("Is this dog reserved? (True or False)");
        boolean reserved = scanner.nextBoolean();scanner.nextLine();
        System.out.println("What country will the dog serve in?");
        String srvCountry = scanner.nextLine();
        Dog newDog = new Dog(name, breed, gender, age, weight, acquireDate, acquireCountry, trainStatus, reserved, srvCountry);
        dogList.add(newDog);//add to list
        return;
        
    }


    	//Monkey intake
        public static void intakeNewMonkey(Scanner scanner) {
        	System.out.println("What is the monkey's name?");
            String name = scanner.nextLine();
            for(Monkey monkey: monkeyList) {
                if(monkey.getName().equalsIgnoreCase(name)) {
                    System.out.println("\n\nThis monkey is already in our system\n\n");
                    return; 
                }
            }
            //Collect monkey info
            System.out.println("What is the monkey's species?");
            String species = scanner.nextLine();
            System.out.println("What is the monkey's gender?");
            String gender = scanner.nextLine();
            System.out.println("What is the monkey's age?");
            String age = scanner.nextLine();
            System.out.println("What is the monkey's weight?");
            String weight = scanner.nextLine();
            System.out.println("When was the monkey acquired?");
            String acquireDate = scanner.nextLine();
            System.out.println("What country was the monkey acuired from?");
            String acquireCountry = scanner.nextLine();
            System.out.println("What is the monkey's training status?");
            String trainStatus = scanner.nextLine();
            System.out.println("Is this monkey reserved? (True or False)");
            boolean reserved = scanner.nextBoolean(); scanner.nextLine();
            System.out.println("What country will the monkey serve in?");
            String srvCountry = scanner.nextLine();
            System.out.println("What is the monkey's tail length?");
            String tailLength = scanner.nextLine();
            System.out.println("What is the monkey's body height?");
            String height = scanner.nextLine();
            System.out.println("What is the monkey's body length?");
            String bodyLength = scanner.nextLine();
            Monkey newMonkey = new Monkey(name, species, gender, age, weight, acquireDate, acquireCountry, trainStatus, reserved, srvCountry, tailLength, height, bodyLength);
            monkeyList.add(newMonkey);
            return;
        }

        //Reserve an animal
        public static void reserveAnimal(Scanner scanner) {
        	String animalType = "";
        	String animalBreed = "";
        	String animalCountry = "";
        	boolean validReserve = false;
        	
        	animalBreed.equalsIgnoreCase(animalBreed);
        	do {
        		System.out.println("Would you like to reserve a dog or monkey?");
        		animalType = scanner.nextLine();
        		try {
        			if (animalType.equalsIgnoreCase("dog")) {
        				System.out.println("What breed is the dog?");
        				animalBreed = scanner.nextLine();
        				System.out.println("Where will the dog be in service?");
        				animalCountry = scanner.nextLine();
        				
        				//reserve dog
        				for (Dog dog: dogList) {
        					if ((dog.getBreed().equalsIgnoreCase(animalBreed)) && (dog.getInServiceLocation().equalsIgnoreCase(animalCountry))) {
        						System.out.println("Would you like to reserve this dog?");
        						String resDog = scanner.nextLine();
        						if(resDog.equalsIgnoreCase("yes")) {
        							dog.setReserved(true);
        						}
        						return;
        					}
        				}
        				scanner.nextLine();
        				validReserve = true;
        				break;
        			}
        			//reserve Monkey
        			else if (animalType.equalsIgnoreCase("monkey")) {
        				System.out.println("What species is the monkey?");
        				animalBreed = scanner.nextLine();
        				System.out.println("Where will the monkey be in service?");
        				animalCountry = scanner.nextLine();
        				for (Monkey monkey: monkeyList) {
        					if ((monkey.getSpecies().equalsIgnoreCase(animalBreed)) && (monkey.getInServiceLocation().equalsIgnoreCase(animalCountry))) {
        						System.out.println("Would you like to reserve this monkey?");
        						String resMonkey = scanner.nextLine();
        						if(resMonkey.equalsIgnoreCase("yes")) {
        							monkey.setReserved(true);
        						}
        						return;
        					}
        				}
        				scanner.nextLine();
        				validReserve = true;
        				break;
        			}
        			//kick back error for invalid entry
        			else {
        				throw new Exception("Invalid input, try again");
        			}
        		} catch(Exception excpt) {
        			System.out.println(excpt.getMessage());
        			validReserve = false;
        			scanner.nextLine();
        		}
        		}while(!validReserve);
        		return;
        }

        //Print list of animals
        public static void printAnimals(char option) {
        	// Print list of dogs
            if (option == '4') {
            	for(Dog dog: dogList) {
            		if ((dog.getTrainingStatus().equalsIgnoreCase("in service")) && dog.getReserved() == false) {
            			System.out.println("Name: " + dog.getName());
            			System.out.println("Training Status: " + dog.getTrainingStatus());
            			System.out.println("Aquisition Country: " + dog.getAcquisitionLocation());
            			System.out.println("Reserve Status: " + dog.getReserved());
            		}
            	}
            }
            // Print list of monkeys
            if (option == '5') {
            	for(Monkey monkey: monkeyList) {
            		if ((monkey.getTrainingStatus().equalsIgnoreCase("in service")) && monkey.getReserved() == false) {
            			System.out.println("Name: " + monkey.getName());
            			System.out.println("Training Status: " + monkey.getTrainingStatus());
            			System.out.println("Aquisition Country: " + monkey.getAcquisitionLocation());
            			System.out.println("Reserve Status: " + monkey.getReserved());
            		}
            	}
            }
            // Print list of all available animals
            else if (option =='6') {
            	System.out.println("List of Available animals:");
            	//print dogs
            	System.out.println("Dogs:");
            	for(Dog dog: dogList) {
            		if ((dog.getTrainingStatus().equalsIgnoreCase("in service")) && dog.getReserved() == false) {
            			System.out.println("Name: " + dog.getName());
            			System.out.println("Training Status: " + dog.getTrainingStatus());
            			System.out.println("Aquisition Country: " + dog.getAcquisitionLocation());
            			System.out.println("Reserve Status: " + dog.getReserved());
            		}
            	}
            	//print monkeys
            	System.out.println("Monkeys:");
            	for(Monkey monkey: monkeyList) {
            		if ((monkey.getTrainingStatus().equalsIgnoreCase("in service")) && monkey.getReserved() == false) {
            			System.out.println("Name: " + monkey.getName());
            			System.out.println("Training Status: " + monkey.getTrainingStatus());
            			System.out.println("Aquisition Country: " + monkey.getAcquisitionLocation());
            			System.out.println("Reserve Status: " + monkey.getReserved());
            		}
            	}
            }     
        }
}

