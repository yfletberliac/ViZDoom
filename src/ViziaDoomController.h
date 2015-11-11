#ifndef __VIZIA_DOOM_CONTROLLER_H__
#define __VIZIA_DOOM_CONTROLLER_H__

#include <string>

#include "./interprocess/ipc/message_queue.hpp"
#include "./interprocess/shared_memory_object.hpp"
#include "./interprocess/mapped_region.hpp"
#include "./process/process.hpp"

namespace bip = boost::interprocess;
namespace bpr = boost::process;

#define DOOM_AMMO_Clip 0
#define DOOM_AMMO_Shell 1
#define DOOM_AMMO_Rocket 2
#define DOOM_AMMO_Cell 3

#define DOOM_WEAPON_Fist 0
#define DOOM_WEAPON_Chainsaw 0
#define DOOM_WEAPON_Pistol 1
#define DOOM_WEAPON_Shotgun 3
#define DOOM_WEAPON_SSG 3
#define DOOM_WEAPON_SuperShotgun 3
#define DOOM_WEAPON_Chaingun 4
#define DOOM_WEAPON_RocketLuncher 5
#define DOOM_WEAPON_Plasma 6
#define DOOM_WEAPON_BFG 7

#define V_ATTACK 0
#define V_USE 1
#define V_JUMP 2
#define V_CROUCH 3
//#define V_TURN180 4
#define V_ALTATTACK 5
#define V_RELOAD 6
#define V_ZOOM 7

#define V_SPEED 8
#define V_STRAFE 9

#define V_MOVERIGHT 10
#define V_MOVELEFT 11
#define V_BACK 12
#define V_FORWARD 13
#define V_RIGHT 14
#define V_LEFT 15
#define V_LOOKUP 16
#define V_LOOKDOWN 17
#define V_MOVEUP 18
#define V_MOVEDOWN 19
//#define V_SHOWSCORES 20

#define V_WEAPON1 21
#define V_WEAPON2 22
#define V_WEAPON3 23
#define V_WEAPON4 24
#define V_WEAPON5 25
#define V_WEAPON6 26
#define V_WEAPON7 27

#define V_WEAPONNEXT 28
#define V_WEAPONPREV 29

#define V_BT_SIZE 30

#define VIZIA_SM_NAME "ViziaSM"

#define VIZIA_MQ_NAME "ViziaMQ"
#define VIZIA_MQ_MAX_MSG_NUM 32
#define VIZIA_MQ_MAX_MSG_SIZE sizeof(ViziaDoomController::MessageCommandStruct)
#define VIZIA_MQ_MAX_CMD_LEN 32

#define VIZIA_MSG_CODE_DOOM_READY 10
#define VIZIA_MSG_CODE_DOOM_TIC 11
#define VIZIA_MSG_CODE_DOOM_CLOSE 12

#define VIZIA_MSG_CODE_READY 0
#define VIZIA_MSG_CODE_TIC 1
#define VIZIA_MSG_CODE_CLOSE 2
#define VIZIA_MSG_CODE_COMMAND 3
#define VIZIA_MSG_CODE_RESET 4
#define VIZIA_MSG_CODE_RESTART 5

class ViziaDoomController {
    
    public:

        struct InputStruct{
            int MS_X;
            int MS_Y;
            bool BT[V_BT_SIZE];
        };

        struct GameVarsStruct{
            int TIC;

            int SCREEN_WIDTH;
            int SCREEN_HEIGHT;

            bool MAP_FINISHED;

            bool PLAYER_DEAD;

            int PLAYER_KILLCOUNT;
            int PLAYER_ITEMCOUNT;
            int PLAYER_SECRETCOUNT;
            int PLAYER_FRAGCOUNT;

            bool PLAYER_ONGROUND;

            int PLAYER_HEALTH;
            int PLAYER_ARMOR;

            int PLAYER_SELECTED_WEAPON;
            int PLAYER_SELECTED_WEAPON_AMMO;

            int PLAYER_AMMO[4];
            bool PLAYER_WEAPON[7];
            bool PLAYER_KEY[3];
        };


        ViziaDoomController();
        ViziaDoomController(std::string iwad, std::string file, std::string map, int screenWidth, int screenHeight);
        ~ViziaDoomController();

        bool init();
        bool close();

        void setScreenSize(int screenWidth, int screenHeight);
        void setGamePath(std::string path);
        void setIwadPath(std::string path);
        void setFilePath(std::string path);
        void setMap(std::string map);
        void setSkill(int skill);

        void setFrameRate(); //TO DO
        void setHUD(); //TO DO
        void setCrosshair(); //TO DO

        bool tic();
        bool update();
        void resetMap(); //TO DO
        void restartGame(); //TO DO

        void sendCommand(std::string command);

        uint8_t* const getScreen();
        InputStruct* const getInput();
        GameVarsStruct* const getGameVars();

        void setMouse(int x, int y);
        void setMouseX(int x);
        void setMouseY(int y);
        void setButtonState(int button, bool state);
        void setKeyState(int key, bool state);
        void toggleButtonState(int button);
        void toggleKeyState(int key);

        int getGameTic();

        int getPlayerKillCount();
        int getPlayerItemCount();
        int getPlayerSecretCount();
        int getPlayerFragCount();

        int getPlayerHealth();
        int getPlayerArmor();

        int getPlayerSelectedWeaponAmmo();
        int getPlayerSelectedWeapon();

        int getPlayerAmmo1();
        int getPlayerAmmo2();
        int getPlayerAmmo3();
        int getPlayerAmmo4();

        bool getPlayerWeapon1();
        bool getPlayerWeapon2();
        bool getPlayerWeapon3();
        bool getPlayerWeapon4();
        bool getPlayerWeapon5();
        bool getPlayerWeapon6();
        bool getPlayerWeapon7();

        bool getPlayerKey1();
        bool getPlayerKey2();
        bool getPlayerKey3();

    private:

        struct MessageSignalStruct{
            uint8_t code;
        };

        struct MessageCommandStruct{
            uint8_t code;
            char command[VIZIA_MQ_MAX_CMD_LEN];
        };

        void SMInit();

        void SMSetSize(int screenWidth, int screenHeight);

        size_t SMGetInputRegionBeginning();
        size_t SMGetGameVarsRegionBeginning();
        size_t SMGetScreenRegionBeginning();

        void SMClose();

        void MQInit();

        void MQSend(uint8_t code);

        bool MQTrySend(uint8_t code);

        void MQSend(uint8_t code, const char * command);

        bool MQTrySend(uint8_t code, const char * command);

        void MQRecv(void *msg, unsigned long &size, unsigned int &priority);

        bool MQTryRecv(void *msg, unsigned long &size, unsigned int &priority);

        void MQClose();

        void waitForDoom();

        void lunchDoom();

        int gameTic;
        int screenWidth, screenHeight, screenSize;

        std::string gamePath;
        std::string iwadPath;
        std::string file;
        std::string map;
        int skill;


        bip::shared_memory_object *SM;
        size_t SMSize;

        bip::message_queue *MQ;

        bip::mapped_region *InputSMRegion;
        InputStruct *Input;

        bip::mapped_region *GameVarsSMRegion;
        GameVarsStruct *GameVars;

        bip::mapped_region *ScreenSMRegion;
        uint8_t *Screen;

        //bpr::child doomProcess;
        bool doomRunning;
};


#endif
