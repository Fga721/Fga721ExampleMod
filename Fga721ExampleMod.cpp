#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <unistd.h>

#include "Logging.h"
#include "MemUtils.h"
#include "SexyTypes.h"
#include "Board.h"
#include "Fga721ExampleMod.h"

// PlantNameMapper: permite crear nuevos ids de plantas

class PlantNameMapper
{
public:
    void* vftable;
    std::map<SexyString, uint> m_aliasToId;
};

typedef PlantNameMapper* (*PlantNameMapperCtor)(PlantNameMapper*);
PlantNameMapperCtor oPlantNameMapperCtor = NULL;

void* hkCreatePlantNameMapper(PlantNameMapper* self)
{
    oPlantNameMapperCtor(self); // deja que el juego cree ids de plantas
    self->m_aliasToId.emplace(SexyString("krishnateamo"), 188u); // crea tus ids nuevos
    return self;
}

// ZombieAlmanac: permite crear nuevos ids al bando de los zombies

class ZombieAlmanac
{
public:
    void* vftable;
    std::map<SexyString, uint> m_aliasToId;
};

typedef ZombieAlmanac* (*ZombieAlmanacCtor)(ZombieAlmanac*);
ZombieAlmanacCtor oZombieAlmanacCtor = NULL;

void* hkCreateZombieTypenameMap(ZombieAlmanac* self)
{
    oZombieAlmanacCtor(self); // misma cosa que las plantas
    self->m_aliasToId.emplace(SexyString("randomid"), 441u);
    return self;
}

// Softcode del zombie piano

typedef bool (*initZombiePianoList)(int64, int64);
initZombiePianoList oInitZombiePianoList = NULL;

std::vector<SexyString>* g_pianoList = NULL;
bool g_pianoListInitialized = false;

bool hkInitZombiePianoList(int64 a1, int64 a2)
{
    bool result = oInitZombiePianoList(a1, a2);

    if (!g_pianoListInitialized)
    {
        g_pianoList = reinterpret_cast<std::vector<SexyString>*>(Fga721GetActualOffset(0x2581BD0));

        g_pianoList->clear();
		g_pianoList->push_back(SexyString("cowboy"));
        g_pianoList->push_back(SexyString("cowboy_armor1"));
        g_pianoList->push_back(SexyString("cowboy_armor2"));
        g_pianoList->push_back(SexyString("cowboy_armor4"));
        g_pianoListInitialized = true;
    }

    return result;
}

// memoria de momia

uint64 oCamelZombieFunc = NULL;

typedef void(*camelMinigameModuleFunc)(int64, int64, char);
camelMinigameModuleFunc cmmFunc = (camelMinigameModuleFunc)Fga721GetActualOffset(0xAC4638);

void* vftable;

enum class ZombieState_CamelTouch {
    ZS_CAMELTOUCH_WaitingToRise = 18,
    ZS_CAMELTOUCH_RisingFromGround = 19,
    ZS_CAMELTOUCH_Stunned = 20
};

// Recreamos la clase del zombiecameltouch (muchas gracias a blazey por este codigo)

class ZombieCamelTouch
{
public:
    char pad1[1112];
    pvztime_t m_riseFromGroundDelay;
    char pad2[28];

    void EnterState(int state, bool a3)
    {
        return CallFunc<void, ZombieCamelTouch*, int, bool>(0xC3D428, this, state, a3);
    }

    virtual void UpdatePosition(Sexy::SexyVector3* newPos) {};

    static void SpawnFromGround(ZombieCamelTouch* thisPtr, Sexy::SexyVector3* groundPos, int delay);
};

static_assert(sizeof(ZombieCamelTouch) == 1152);
static_assert(offsetof(ZombieCamelTouch, m_riseFromGroundDelay) == 1120);

// funcion que hace que el zombie salga del suelo, con un delay y una posicion especifica

void ZombieCamelTouch::SpawnFromGround(ZombieCamelTouch* thisPtr, Sexy::SexyVector3* groundPos, int delay)
{
    thisPtr->m_riseFromGroundDelay = delay;
    thisPtr->UpdatePosition(groundPos);
    thisPtr->EnterState((int)ZombieState_CamelTouch::ZS_CAMELTOUCH_WaitingToRise, false);
}

// hacemos que la funcion vaya hasta otra funcion colchon para que almacene la llamada original, primero ejecutamos la funcion de la salida de los camellos del suelo

void hkCamelZombieFunc(int64 a1, int64 a2, char a3)
{
	ZombieCamelTouch::SpawnFromGround(reinterpret_cast<ZombieCamelTouch*>(a1), reinterpret_cast<Sexy::SexyVector3*>(a2), a3);
    cmmFunc(a1, a2, a3);
}

// Inmunidades del mago y el curandero al encogimiento harcodeadas de forma tonta (no funcionan muy bien porque no encontre los offsets de las inmunidades a encantamenta)

uint64 header;

void hkMagicianConditionFunc(int64 a1, int64 condition)
{
    return;
}

bool hkMagicianImmuneToShrink(int64_t a1)
{
    // true = Mago Inmune al encogimiento
    return false;
}

void hkMagicianInitializeFamilyImmunities(int64_t a1, int64_t a2)
{
    return;
}

// Hookeo de zanahoria intensiva

typedef bool (*checkIntensiveCarrotShowPlant)(int64, int64);
checkIntensiveCarrotShowPlant oCheckIntensiveCarrotShowPlant = NULL;

bool hkCheckIntensiveCarrotShowPlant(int64 r0_0, int64 plant) {
    return oCheckIntensiveCarrotShowPlant(r0_0, plant);
}

__attribute__((constructor))
void libFga721_main()
{
    // incializar hooks
    Fga721HookFunction(0x11797B4, (void*)hkCreatePlantNameMapper, (void**)&oPlantNameMapperCtor);
    Fga721HookFunction(0x14665C4, (void*)hkCreateZombieTypenameMap, (void**)&oZombieAlmanacCtor);
    Fga721HookFunction(0xB18DC4, (void*)hkCamelZombieFunc, (void**)&oCamelZombieFunc);
    Fga721HookFunction(0xC1D1FC, (void*)hkInitZombiePianoList, (void**)&oInitZombiePianoList);
    Fga721HookFunction(0xBF7BD4, (void*)hkMagicianConditionFunc, (void**)&header);
    Fga721HookFunction(0xC12B7C, (void*)hkMagicianImmuneToShrink, (void**)&header);
    Fga721HookFunction(0xBF7BEC, (void*)hkMagicianInitializeFamilyImmunities, (void**)&header);
    Fga721HookFunction(0xE431C4, (void*)hkCheckIntensiveCarrotShowPlant, (void**)&oCheckIntensiveCarrotShowPlant);
    // Fga721HookFunction(0xBD873C, (void*)hkHealerConditionFunc, (void**)&header);
    // Fga721HookFunction(0xC0F91C, (void*)hkHealerImmuneToShrink, (void**)&header);

    LOGI("Finished initializing");
}
