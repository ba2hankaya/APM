#include <iostream>
#include <string>
#include <vector>

class Item;
class Player;

class ItemPresetDatabase
{
  private:
    ItemPresetDatabase(); //private constructor
    static ItemPresetDatabase *instance;
    std::vector<Item*> presetList;  //list of item pointers
  public:
    static ItemPresetDatabase *getInstance(){ //get instance method
      if(instance == NULL)//singleton implementation
      {
        instance = new ItemPresetDatabase();
      }
      return instance; //return instant
    }
    const std::vector<Item*>& getPresetList() const {return presetList;} //return a constant reference to the presetlist of the static database
};
ItemPresetDatabase *ItemPresetDatabase::instance = NULL;//singleton implementation

class Item
{  
  private:
    int id; 
    int maxDurability;
    std::string name;
    std::string description;
    int currentDurability;
    //every single item has these

    int quantity=0;//used for creating instances that will be used in the inventory
    const ItemPresetDatabase* database = nullptr; // pointer to the database
    const std::vector<Item*>* presetList = nullptr; //pointer to the presetlist
 
    public:
    
    Item(const int &id, const int& maxDurability, const std::string &name, const std::string &description)
    {//item constructor for presets
      this->id = id;
      this->maxDurability = maxDurability;
      this->name = name;
      this->description = description;
      this->currentDurability = maxDurability;
    }


    virtual Item* cloneQuantityOfPreset(const int &quantity) const//returns a clone of this with given quantity(an instance)
    {
      Item* toReturn = new Item(*this);
      toReturn->changeQuantity(quantity);
      return toReturn;
    }

    std::string getName() const {return name;}

    int getId() const {return id;}

    int getMaxDurability() const {return maxDurability;}

    std::string getDescription() const {return description;}

    int getCurrentDurability() const{return currentDurability;}

    int getQuantity() const{return quantity;}
    //get methods


    void changeQuantity(const int& by){quantity += by;}
    //change quantity method

    //overrideable use method to be used for instances
    virtual void use()
    {
      currentDurability--; //lower durability by one
      if(currentDurability <= 0)
      {
        quantity--; //if durabiliity is zero lower quantity(inventory removes the item ifk quantity = 0) add a luck feature here
        if(quantity != 0)
        {
          currentDurability = maxDurability;//after quantity drops, if the quantity isn't zero reset the durability
        }
      }
    }
};

class Player
{
	private:
    double currentHealth;
		double maxHealth;
		double currentEnergy;
		double maxEnergy;
		double attack = 1;
    double luck = 0;
    double defense = 0;
    //player stats
    
    const ItemPresetDatabase* database = nullptr; // pointer to the database
    const std::vector<Item*>* presetList = nullptr; //pointer to the presetlist
    //variables for database and presetlist

    static Player *instance;

    std::vector<Item*> inventory = std::vector<Item*>(10, nullptr);
    //inventory is a vector of item pointers with 10 slots all initially set to null

    //player construct in private (singleton)
    Player(){
		  maxEnergy = 30;//initial values of health and energyy of the player at the start of the game
			maxHealth = 100;
      currentHealth = maxHealth;
      currentEnergy = maxEnergy;
      database = ItemPresetDatabase::getInstance();
      presetList = &database->getPresetList();
		}

	  public:
	    static Player *getInstance(){//get player intance
        if(instance == NULL)
        {
          instance = new Player();
        }
        return instance;
      }
		
    void SetHealthTo(const double& setTo){
		  currentHealth = setTo;
		}

    void changeHealthBy(const double& by){
      std::cout << by;
      currentHealth += by;
      std::cout << currentHealth;
    }

    double getCurrentHealth() const{return currentHealth;}

    double getMaxHealth() const{return maxHealth;}

    void SetEnergyTo(const double& setTo){
      currentEnergy = setTo;
    }

    void changeEnergyBy(const double& by){
      currentEnergy += by;
    }

    double getCurrentEnergy() const {return currentEnergy;}

    double getMaxEnergy() const {return maxEnergy;}

    double getAttack() const {return attack;}

    double getLuck() const {return luck;}

    double getDefense() const {return defense;}

    void changeAttackBy(const double &by)
    {
      attack += by;
    }

    void changeLuckBy(const double &by)
    {
      luck += by;
    }

    void changeDefenseBy(const double &by)
    {
      defense += by;
    }

    int addItem(const int& id, const int& quantity) //add quantity of item with id
    {
       //if item is already present in the inventory, increase its count and return 1
       for(int i = 0; i < inventory.size(); i++)
       {
          if(inventory[i] != nullptr && inventory[i]->getId() == id) //only try to call method if it isn't nullptr, if item of the same id exists, increase its count
          {
            inventory[i]->changeQuantity(quantity);
            return 1;
          }
       }
        
       //if it is not present in the inv, find the preset, then point to its clone in an empty slot
       const Item* preset = nullptr;
       for(int i = 0; i < presetList->size(); i++)
       {
          if((*presetList)[i]->getId() == id)
          {
            preset = (*presetList)[i];
          }
       }
       for(int i = 0; i < inventory.size(); i++) // if item of the same id doesn't exist, add instance of item with quantity
       {
          if(inventory[i] == nullptr) //check for null ptr
          {
            inventory[i] = preset->cloneQuantityOfPreset(quantity);
            return 1;
          }
       }
       return -1;//return -1 if item couldn't be added.
    }

    int useItem(const int& index)
    {
      if(inventory[index] == nullptr)
      {
        std::cout << "you tried to use an empty slot\n"; // error msg
        return -1; //return -1 if nullptr was trying to be use
      }
      Item &itemref = *inventory[index];//get a reference to the pointed item after confirming it isn't a nullptr POTENTIAL ISSUE FOR CASTING IF THE REFERENCE IS A POTION
      std::cout << "Using item: " << itemref.getName() << " (Type: " << typeid(itemref).name() << ")" << std::endl; // Debug output
      itemref.use(); //call use method
      if(itemref.getQuantity() == 0) //if item quantity is 0 after being used, remove item from inventory (set pointer to nullptr)
      {
        inventory[index] = nullptr;
      }
      return 1;
    }

    void listInventory()
    {
      for(int i = 0; i < inventory.size(); i++)
      {
        if(inventory[i] != nullptr) // if it isn't a null pointer, print info, else print "empty slot"
        {
          const Item& item = *inventory[i];
          std::cout << "Name:\t" << item.getName() << "\tQuantity:\t" << std::to_string(item.getQuantity()) << "\tDescription:\t" << item.getDescription() << "\tDurability:\t" << std::to_string(item.getCurrentDurability()) << "/" << std::to_string(item.getMaxDurability()) << "\n";
        }else
        {
          std::cout << "Empty Slot \n"; 
        }
      }
    }
};
Player *Player::instance = NULL;

class Potion : public Item
{
  public:
    enum PotionType //enum for pot type
    {
      Healing,
      Energy
    };
  private:
  //unique vars for potions
    PotionType potionType;
    int effectAmount;
  public:
    Potion(const int &id, const int& maxDurability, const std::string &name, const std::string &description, PotionType potionType, int effectAmount) : Item(id, maxDurability, name, description)
    {//potion constructor for presets
      this->potionType = potionType;
      this->effectAmount = effectAmount;
    }

    void use() override
    {//overwrite item use
      Item::use();//call the item use method
      Player* player = Player::getInstance(); //get player singleton
      if(potionType == Healing)//if type == healing heal if it is energy increase energy
      {
        player->changeHealthBy(effectAmount);
        std::cout << effectAmount;
      }else if(potionType == Energy)
      {
        player->changeEnergyBy(effectAmount);
      }
    }

    Item* cloneQuantityOfPreset(const int& quantity) const override
    {//overwrite clone method so it return potion type instead of item
      Potion* toReturn = new Potion(*this);
      toReturn->changeQuantity(quantity);
      return toReturn;
    }

    PotionType getPotionType() const {return potionType;}
    int getEffectAmount() const {return effectAmount;}
   
};

class Armor : Item // here just for now
{
  public:
    enum ArmorType//for slots
    {
      Headgear,
      Chestgear,
      Pants,
      Shoes,
      Orb,
      Earrings,
      Rune,
      Cape,
      Belt,
      Wristband
    };
  private:
    double armorincrease = 0;//effects
    double atkincrease = 0;
    double luckincrease = 0;
};

ItemPresetDatabase::ItemPresetDatabase()
{//item presets
  presetList.push_back(new Item(0, 4 , "Sword." , "This is a sword"));
  presetList.push_back(new Potion(1, 1, "10HP Potion", "Restores 10hp" , Potion::Healing, 10)); 
}

int main(int argc, char* argv[])
{
  Player* p = Player::getInstance();
  p->SetHealthTo(50);
  std::cout << std::to_string(p->getCurrentHealth()) << "\n";
  const ItemPresetDatabase* database = ItemPresetDatabase::getInstance();
  const std::vector<Item*>& presetList = database->getPresetList();
  std::cout << presetList[0]->getName() << "\n";
  p->addItem(0 , 2);
  p->listInventory();
  p->useItem(0);
  p->listInventory();
  p->useItem(0);
  p->listInventory();
  p->useItem(0);
  p->listInventory();
  p->useItem(0);
  p->listInventory();
  p->useItem(0);
  p->listInventory();
  //p.addItem(0 ,1);
  p->useItem(0);
  p->listInventory();
  p->useItem(0);
  p->listInventory();
  p->useItem(0);
  p->listInventory();
  p->useItem(0);
  p->listInventory();
  p->useItem(0);
  p->listInventory();
  p->addItem(1, 1);
  p->listInventory();
  p->useItem(0);
  p->listInventory();
  p->changeHealthBy(20);
  std::cout << std::to_string(p->getCurrentHealth()) << "\n";
}
