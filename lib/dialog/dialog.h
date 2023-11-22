#ifndef DIALOG_H
#define DIALOG_H

#include <memory>
#include <vector>
#include <string>


struct Quest
{
    std::string quest;
    std::string response;
};

class Dialog
{
    private:
        std::unique_ptr<std::vector<Quest>> quests;
    public:
        Dialog(std::vector<Quest>& quests);
        virtual ~Dialog();
};

#endif