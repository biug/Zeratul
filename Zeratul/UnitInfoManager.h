#pragma once

#include "sc2api/sc2_api.h"

#include "UnitData.h"
#include "BaseLocation.h"

class Zeratul;
class UnitInfoManager 
{
    Zeratul &           m_bot;

    std::map<int, UnitData> m_unitData;

    std::map<int, std::vector<const sc2::Unit *>> m_units;

    void                    updateUnit(const sc2::Unit * unit);
    void                    updateUnitInfo();
    bool                    isValidUnit(const sc2::Unit * unit);
    
    const UnitData &        getUnitData(int player) const;

    void drawSelectedUnitDebugInfo();

public:

    UnitInfoManager(Zeratul & bot);

    void                    onFrame();
    void                    onStart();

    const std::vector<const sc2::Unit *> & getUnits(int player) const;

    size_t                  getUnitTypeCount(int player, sc2::UnitTypeID type, bool completed = true) const;

    void                    getNearbyForce(std::vector<UnitInfo> & unitInfo, sc2::Point2D p, int player, float radius) const;

    const std::map<const sc2::Unit *, UnitInfo> & getUnitInfoMap(int player) const;

    //bool                  enemyHasCloakedUnits() const;
    void                    drawUnitInformation(float x, float y) const;

};