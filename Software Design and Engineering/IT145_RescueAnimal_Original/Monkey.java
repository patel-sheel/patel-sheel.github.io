
public class Monkey extends RescueAnimal {
	//Instance Fields
	private String tailLength;
	private String height;
	private String bodyLength;
	private String species;
	
	//Constructor
	public Monkey(String name, String species, String gender, String age,String weight, String acquisitionDate, String acquisitionCountry, 
	String trainingStatus, boolean reserved, String inServiceCountry, String tailLength, String height, String bodyLength) { 
        setSpecies(species);
        setTailLength(tailLength);
        setHeight(height);
        setBodyLength(bodyLength);
}
	
	//Accessors and Mutators
	public void setTailLength(String tailLength) {
		this.tailLength = tailLength;
	}
	public String getTailLength() {
		return tailLength;
	}
	public void setHeight(String height) {
		this.height = height;
	}
	public String getHeight() {
		return height;
	}
	public void setBodyLength(String bodyLength) {
		this.bodyLength = bodyLength;
	}
	public String getBodyLength() {
		return bodyLength;
	}
	public void setSpecies(String species) {
		this.species = species;
	}
	public String getSpecies() {
		return species;
	}
}
