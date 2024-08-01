/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2023
* Author: Vu Van Tien
* Date: 02.02.2023
*/

//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class MovingObject;
class Position;
class Configuration;
class Map;

class Criminal;
class RobotS;
class RobotW;
class RobotSW;
class RobotC;

class ArrayMovingObject;
class StudyPinkProgram;

class BaseItem;
class BaseBag;
class SherlockBag;
class WatsonBag;

class TestStudyInPink;

enum ItemType { MAGIC_BOOK, ENERGY_DRINK, FIRST_AID, EXCEMPTION_CARD, PASSING_CARD };
enum ElementType { PATH, WALL, FAKE_WALL };
enum RobotType { C=0, S, W, SW };


class MapElement {
protected:
    ElementType type;
public:
friend class TestStudyInPink;
    MapElement(ElementType in_type);
    virtual ~MapElement();
    virtual ElementType getType() const;
};

class Path : public MapElement {
public:    
    Path() : MapElement(PATH){}
};

class Wall : public MapElement {
public:    
    Wall() : MapElement(WALL) {}
};

class FakeWall : public MapElement {
private:
    int req_exp;
public:
friend class TestStudyInPink;
    FakeWall(int in_req_exp) : MapElement(FAKE_WALL) {
        this->req_exp = in_req_exp;
    }
    int getReqExp () const {
        return req_exp;
    }
};


class Map {
private:
    int num_rows, num_cols;
    MapElement*** map;

public:
friend class TestStudyInPink;
    Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls);
    ~Map();
   
    bool isValid( const Position & pos , MovingObject * mv_obj ) const ;
};

class Position {
private:
    int r, c;
public:
friend class TestStudyInPink;
    static const Position npos;
    Position(int r=0, int c=0);
    Position(const string & str_pos);
    int getRow() const;
    void setRow(int r);
    
    int getCol() const;
    void setCol(int c);
    
    string str() const;
    bool isEqual(int in_r, int in_c) const;
};


class MovingObject {
public:
    int index;
    Position pos;
    Map * map;
    string name;
public:
friend class TestStudyInPink;
    MovingObject(int index, const Position pos, Map * map, const string & name="");
    virtual ~MovingObject();

    virtual Position getNextPosition() = 0;
    Position getCurrentPosition() const {
        return pos;
    }
    virtual void move() = 0;
    virtual string str() const = 0;
};



class Character : public MovingObject {
public:
    int hp;
    int exp;
    int nums_move;
public: 
friend class TestStudyInPink;
    Character(int index, const Position pos, Map * map, const string & name, int init_hp=0, int init_exp=0);
    ~Character();
    void setHp(int in_hp);
    void setExp(int in_exp);
    string getName() const;
    void editPosition(Position newPos) {
        pos = newPos;
    }
    int getDistanceToPosition(Position other_pos) const;
};

class Sherlock : public Character {
private:
    // TODO
    string moving_rule;
public:
friend class TestStudyInPink;
    Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp);
    Position getNextPosition() override;
    void move() override;
    string str() const override;
};

class Watson : public Character {
private:
    string moving_rule;
public:
friend class TestStudyInPink;
    Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp);
    Position getNextPosition() override;
    void move() override;
    string str() const override;
    
};

class Criminal : public Character {
private:
    // TODO
    Sherlock* sherlock;
    Watson* watson;
    int valid_move;
    Position pre_position;
public:
friend class TestStudyInPink;
    Criminal(int index, const Position & init_pos, Map * map, Sherlock * sherlock, Watson * watson);
    Position getNextPosition() override;
    void move() override;
    string str() const override;
    int getValidMove() const;
    int dis_to_S_and_W(int r, int c);
    Position getPrePosition();
};

class ArrayMovingObject {
private:
    // TODO
    int count;
    int capacity;
    MovingObject** arr_mv_objs;
public:
friend class TestStudyInPink;
    ArrayMovingObject(int capacity);

    ~ArrayMovingObject() ;
    bool isFull() const;
    bool add(MovingObject * mv_obj);
    MovingObject * get(int index) const;
    int size() const; // return current number of elements in the array
    string str() const;
};

class Configuration {
    friend class StudyPinkProgram;
private:
    int map_num_rows, map_num_cols;
    int max_num_moving_objects;
    int num_walls;
    Position* arr_walls; 
    int num_fake_walls;
    Position* arr_fake_walls;
    string sherlock_moving_rule;
    Position sherlock_init_pos;
    int sherlock_init_hp;
    int sherlock_init_exp;
    string watson_moving_rule;
    Position watson_init_pos;
    int watson_init_hp;
    int watson_init_exp;
    Position criminal_init_pos;
    int num_steps; 

public:
friend class TestStudyInPink;
    Configuration(const string & filepath);
    ~Configuration();
    string str() const;
};

// Robot, BaseItem, BaseBag,...


class Robot : public MovingObject {
protected:
    RobotType robot_type;
    BaseItem* item;
public: 
friend class TestStudyInPink;
    Robot(RobotType robot_type, int index, const Position pos, Map * map);
    ~Robot();
    RobotType getRobotType() {
        return robot_type;
    }
    BaseItem* getItem();
};


class RobotC : public Robot {
private:
    Criminal* criminal;
public:
friend class TestStudyInPink;
    RobotC(int index , const Position & init_pos , Map * map , Criminal *criminal);
    ~RobotC();
    Position getNextPosition();
    void move();
    int getDistance(Sherlock* Sherlock) const;
    int getDistance(Watson*watson) const;
    string str() const;
};

class RobotS : public Robot {
private:
    Criminal* criminal;
    Sherlock* sherlock;
public: 
friend class TestStudyInPink;
    RobotS( int index , const Position & init_pos , Map * map , Criminal *criminal , Sherlock * sherlock );
    ~RobotS();
    Position getNextPosition();
    void move();
    int getDistance() const;
    string str() const;
};

class RobotW : public Robot {
private:    
    Criminal* criminal;
    Watson* watson;
public: 
friend class TestStudyInPink;
    RobotW( int index , const Position & init_pos , Map * map , Criminal *criminal , Watson * watson );
    ~RobotW();
    Position getNextPosition();
    void move();
    int getDistance() const;
    string str() const;
};

class RobotSW : public Robot {
private:
    Criminal* criminal;
    Sherlock* sherlock;
    Watson* watson;

public:
friend class TestStudyInPink;
    RobotSW(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson);
    ~RobotSW();
    Position getNextPosition();
    void move();
    int getDistance() const;
    string str() const;
};

// sub base item


class BaseItem {
public:    
    BaseItem* next;
    ItemType item_type;
public:
friend class TestStudyInPink;
    BaseItem(ItemType item_type);
    ~BaseItem();
    virtual bool canUse ( Character * obj , Robot * robot ) = 0;
    virtual void use( Character * obj , Robot * robot ) = 0;
};


class MagicBook : public BaseItem {

public:
    MagicBook() : BaseItem(MAGIC_BOOK) {}
    ~MagicBook(){}
    bool canUse(Character* obj, Robot* robot) override;
    void use(Character* obj, Robot* robot) override;
};

class EnergyDrink : public BaseItem {

public:
    EnergyDrink() : BaseItem(ENERGY_DRINK) {}
    ~EnergyDrink(){}
    bool canUse(Character* obj, Robot* robot) override;
    void use(Character* obj, Robot* robot) override;
};

class FirstAid : public BaseItem {

public:
    FirstAid() : BaseItem(FIRST_AID) {}
    ~FirstAid() {}
    bool canUse(Character* obj, Robot* robot) override;
    void use(Character* obj, Robot* robot) override;
};

class ExcemptionCard : public BaseItem {

public:
    ExcemptionCard() : BaseItem(EXCEMPTION_CARD) {}
    ~ExcemptionCard(){}
    bool canUse(Character* obj, Robot* robot) override;
    void use(Character* obj, Robot* robot) override;
};

class PassingCard : public BaseItem {
public:
    string challenge;
public:
friend class TestStudyInPink;
    PassingCard(const string & challenge);
    ~PassingCard();
    bool canUse(Character* obj, Robot* robot) override;
    void use(Character* obj, Robot* robot) override;
};


class BaseBag {
public:
    Character* obj;
public:
    BaseBag(Character* obj) : obj(obj) {}
    ~BaseBag(){}

    virtual bool insert ( BaseItem * item ) {
        return false;
    }
    virtual BaseItem * get (){ 
        return nullptr;
    }
    virtual BaseItem * get( ItemType itemType ) {
        return nullptr;
    }
    virtual string str () const {
        return "";
    }

};

class SherlockBag : public BaseBag {
private:
    BaseItem* head;
    int count;

public:
friend class TestStudyInPink;
    SherlockBag(Sherlock* sherlock);
    ~SherlockBag();
    int numsOfItem(ItemType itemType);
    bool insert(BaseItem* item) override;
    BaseItem * get() override;
    BaseItem * get(ItemType itemType) override;
    BaseItem * canUseItem();
    string str() const override;
};
class WatsonBag : public BaseBag {
private:
    BaseItem* head;
    int count;
public:
friend class TestStudyInPink;
    WatsonBag(Watson* watson);
    ~WatsonBag();
    int numsOfItem(ItemType itemType) const;
    bool insert(BaseItem* item) override;
    BaseItem * get() override;
    BaseItem * get(ItemType itemType) override;
    BaseItem * canUseItem();
    string str() const override;
};

class StudyPinkProgram {
private:
    // Sample attributes
    Configuration * config;
    Sherlock * sherlock;
    Watson * watson;
    Criminal * criminal;
    Map * map;
    ArrayMovingObject * arr_mv_objs;
    // Bag
    SherlockBag* sherlockBag;
    WatsonBag* watsonBag;
    
public:
friend class TestStudyInPink;
    StudyPinkProgram(const string & config_file_path) {
        config = new Configuration(config_file_path);
        map = new Map(config->map_num_rows, config->map_num_cols, config->num_walls, config->arr_walls, config->num_fake_walls, config->arr_fake_walls);
        
        sherlock = new Sherlock(1, config->sherlock_moving_rule, config->sherlock_init_pos, map, config->sherlock_init_hp, config->sherlock_init_exp);
        watson = new Watson(2, config->watson_moving_rule, config->watson_init_pos, map, config->watson_init_hp, config->watson_init_exp);
        criminal = new Criminal(0, config->criminal_init_pos, map, sherlock, watson);
        arr_mv_objs = new ArrayMovingObject(config->max_num_moving_objects);
        sherlockBag = new SherlockBag(sherlock);
        watsonBag = new WatsonBag(watson);

        arr_mv_objs->add(criminal);
        arr_mv_objs->add(sherlock);
        arr_mv_objs->add(watson);
    }

    void createRobotMove();
    bool isStop() const;
    void watsonConflict();
    void robotConflict(MovingObject* obj);
    void sherlockConflict();

    void printResult() const {
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(), criminal->getCurrentPosition().getCol())) {
            cout << "Sherlock caught the criminal" << endl;
        }
        else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(), criminal->getCurrentPosition().getCol())) {
            cout << "Watson caught the criminal" << endl;
        }
        else {
            cout << "The criminal escaped" << endl;
        }
    }

    void printStep(int si) const {
        cout << "Step: " << setw(4) << setfill('0') << si
            << "--"
            << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
    }

    void run(bool verbose) {
        for (int istep = 0; istep < config->num_steps; ++istep) {
            for (int i = 0; i < arr_mv_objs->size(); ++i) {
                Position prePos = arr_mv_objs->get(i)->getCurrentPosition();
                arr_mv_objs->get(i)->move();
                Position currPos = arr_mv_objs->get(i)->getCurrentPosition();
                if(!prePos.isEqual(currPos.getRow(), currPos.getCol())) {
                    if(arr_mv_objs->get(i) == criminal) {}
                    else if(arr_mv_objs->get(i) == sherlock) {
                        sherlockConflict();
                    }
                    else if(arr_mv_objs->get(i) == watson) {
                        watsonConflict();
                    }   
                    else {
                        robotConflict(arr_mv_objs->get(i));
                    }
                }
                if (isStop()) {
                    printStep(istep);
                    printResult();
                    return;
                }
                if (verbose) {
                    printStep(istep);
                }
                if(criminal == arr_mv_objs->get(i) && !currPos.isEqual(prePos.getRow(), prePos.getCol())) {
                    createRobotMove();
                }
            }
        }
       
        printResult();
    }


    ~StudyPinkProgram() {
        delete arr_mv_objs;
        delete sherlock;
        delete watson;
        delete criminal;
        delete map;
        delete sherlockBag;
        delete watsonBag;
    }
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
