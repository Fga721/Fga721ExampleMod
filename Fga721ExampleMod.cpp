#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <unistd.h>

#include "Logging.h"
#include "MemUtils.h"
#include "SexyTypes.h"
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

// TEMP del softcode del zombie piano

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

// Inmunidades del mago y el curandero al encogimiento harcodeadas de forma tonta (no funcionan muy bien porque no encontre los offsets de las inmunidades a encantamenta)

uint64 header;

bool hkMagicianHealerImmuneToShrink(int64_t a1)
{
    // true = Mago y Curandero Inmune al encogimiento
    return false;
}

__attribute__((constructor))
void libFga721_main()
{
    // incializar hooks
    Fga721HookFunction(0x11797B4, (void*)hkCreatePlantNameMapper, (void**)&oPlantNameMapperCtor);
    Fga721HookFunction(0x14665C4, (void*)hkCreateZombieTypenameMap, (void**)&oZombieAlmanacCtor);
    Fga721HookFunction(0xC1D1FC, (void*)hkInitZombiePianoList, (void**)&oInitZombiePianoList);
    Fga721HookFunction(0xC12B7C, (void*)hkMagicianHealerImmuneToShrink, (void**)&header);
    Fga721HookFunction(0xC0F91C, (void*)hkMagicianHealerImmuneToShrink, (void**)&header);

    LOGI("Finished initializing");
}