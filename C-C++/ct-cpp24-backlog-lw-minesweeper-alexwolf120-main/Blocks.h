#ifndef _BLOCK_OBJECT_H_
#define _BLOCK_OBJECT_H_

class Blocks
{
public:
    enum Status {
        Normal,
        Clicked,
        Flaged,
        Explode,
        middle,
        Question
    };

    Blocks();
    ~Blocks();

    void setStatus(Status _status);
    Status getStatus();

    void setSweep(bool _sweep);
    bool getSweeped();

    void setFlag(bool _flag);
    bool getFlag();

    void setNearBy(int _nearBy);
    int getNearBy();

    void setMine(bool _isMine);
    bool getMine();

    void setQuestioned(bool _questioned);
    bool getQuestioned();

    void setLocked(bool _locked);
    bool getLocked();

private:
    bool sweeped;
    bool flaged;
    bool isMine;
    int nearBy;
    bool questioned;
    Status status;
    bool locked;
};

#endif
