#include "Squad.h"
#include "Zeratul.h"
#include "Util.h"

Squad::Squad(Zeratul & bot)
	: m_bot(bot)
	, m_lastRetreatSwitch(0)
	, m_lastRetreatSwitchVal(false)
	, m_priority(0)
	, m_name("Default")
	, m_meleeManager(bot)
	, m_rangedManager(bot)
	, m_stalkerManager(bot)
	, m_marineManager(bot)
{
}

Squad::Squad(const std::string & name, const SquadOrder & order, size_t priority, Zeratul & bot)
    : m_bot(bot)
    , m_name(name)
    , m_order(order)
    , m_lastRetreatSwitch(0)
    , m_lastRetreatSwitchVal(false)
    , m_priority(priority)
    , m_meleeManager(bot)
    , m_rangedManager(bot)
	, m_stalkerManager(bot)
	, m_marineManager(bot)
{
}

void Squad::onFrame()
{
    // update all necessary unit information within this squad
    updateUnits();

    // determine whether or not we should regroup
    bool needToRegroup = needsToRegroup();
    
    // if we do need to regroup, do it
    if (needToRegroup)
    {
        sc2::Point2D regroupPosition = calcRegroupPosition();

        m_bot.Map().drawSphere(regroupPosition, 3, sc2::Colors::Purple);

        m_meleeManager.regroup(regroupPosition);
        m_rangedManager.regroup(regroupPosition);
		m_stalkerManager.regroup(regroupPosition);
		m_marineManager.regroup(regroupPosition);
    }
    else // otherwise, execute micro
    {
        m_meleeManager.execute(m_order);
        m_rangedManager.execute(m_order);
		m_stalkerManager.execute(m_order);
		m_marineManager.execute(m_order);
        //_detectorManager.setUnitClosestToEnemy(unitClosestToEnemy());
        //_detectorManager.execute(_order);
    }
}

bool Squad::isEmpty() const
{
    return m_units.empty();
}

size_t Squad::getPriority() const
{
    return m_priority;
}

void Squad::setPriority(const size_t & priority)
{
    m_priority = priority;
}

void Squad::updateUnits()
{
    setAllUnits();
    setNearEnemyUnits();
    addUnitsToMicroManagers();
}

void Squad::setAllUnits()
{
    // clean up the _units vector just in case one of them died
    std::set<const sc2::Unit *> goodUnits;
    for (auto unit : m_units)
    {
        if (!unit) { continue; }
        if (unit->build_progress < 1.0f) { continue; }
        if (unit->health <= 0) { continue; }
        
        goodUnits.insert(unit);
    }

    m_units = goodUnits;
}

void Squad::setNearEnemyUnits()
{
    m_nearEnemy.clear();
    for (auto unit : m_units)
    {
        m_nearEnemy[unit] = isUnitNearEnemy(unit);

        sc2::Color color = m_nearEnemy[unit] ? m_bot.Config().ColorUnitNearEnemy : m_bot.Config().ColorUnitNotNearEnemy;
        //m_bot.Map().drawSphereAroundUnit(unitTag, color);
    }
}

void Squad::addUnitsToMicroManagers()
{
    std::vector<const sc2::Unit *> meleeUnits;
    std::vector<const sc2::Unit *> rangedUnits;
    std::vector<const sc2::Unit *> detectorUnits;
    std::vector<const sc2::Unit *> transportUnits;
	std::vector<const sc2::Unit *> stalkerUnits;
	std::vector<const sc2::Unit *> marineUnits;
    // add _units to micro managers
    for (auto unit : m_units)
    {
        BOT_ASSERT(unit, "null unit in addUnitsToMicroManagers()");

        if (Util::IsDetector(unit) && !m_bot.Data(unit->unit_type).isBuilding)
        {
            detectorUnits.push_back(unit);
        }
		// stalker
		else if (unit->unit_type.ToType() == sc2::UNIT_TYPEID::PROTOSS_STALKER)
		{
			stalkerUnits.push_back(unit);
		}
		else if (unit->unit_type.ToType() == sc2::UNIT_TYPEID::TERRAN_MARINE)
		{
			marineUnits.push_back(unit);
		}
        // select ranged _units
        else if (Util::GetAttackRange(unit->unit_type, m_bot) >= 1.5f)
        {
            rangedUnits.push_back(unit);
        }
        // select melee _units
        else if (Util::GetAttackRange(unit->unit_type, m_bot) < 1.5f)
        {
            meleeUnits.push_back(unit);
        }
    }

    m_meleeManager.setUnits(meleeUnits);
    m_rangedManager.setUnits(rangedUnits);
	m_stalkerManager.setUnits(stalkerUnits);
	m_marineManager.setUnits(marineUnits);
    //m_detectorManager.setUnits(detectorUnits);
    //m_tankManager.setUnits(tankUnits);
}

// TODO: calculates whether or not to regroup
bool Squad::needsToRegroup() const
{
    return false;
}

void Squad::setSquadOrder(const SquadOrder & so)
{
    m_order = so;
}

bool Squad::containsUnit(const sc2::Unit * unit) const
{
    return std::find(m_units.begin(), m_units.end(), unit) != m_units.end();
}

void Squad::clear()
{
    for (auto unit : getUnits())
    {
        BOT_ASSERT(unit, "null unit in squad clear");

        if (Util::IsWorker(unit))
        {
            m_bot.Workers().finishedWithWorker(unit);
        }
    }

    m_units.clear();
}

bool Squad::isUnitNearEnemy(const sc2::Unit * unit) const
{
    BOT_ASSERT(unit, "null unit in squad");

    for (auto & u : m_bot.Observation()->GetUnits())
    {
        if ((Util::GetPlayer(u) == Players::Enemy) && (Util::Dist(unit->pos, u->pos) < 20))
        {
            return true;
        }
    }

    return false;
}

sc2::Point2D Squad::calcCenter() const
{
    if (m_units.empty())
    {
        return sc2::Point2D(0.0f,0.0f);
    }

    sc2::Point2D sum(0,0);
    for (auto unit: m_units)
    {
        BOT_ASSERT(unit, "null unit in squad calcCenter");
        sum += unit->pos;
    }

    return sc2::Point2D(sum.x / m_units.size(), sum.y / m_units.size());
}

sc2::Point2D Squad::calcRegroupPosition() const
{
    sc2::Point2D regroup(0.0f,0.0f);

    float minDist = std::numeric_limits<float>::max();

    for (auto unit : m_units)
    {
        if (!m_nearEnemy.at(unit))
        {
            float dist = Util::Dist(m_order.getPosition(), unit->pos);
            if (dist < minDist)
            {
                minDist = dist;
                regroup = unit->pos;
            }
        }
    }

    if (regroup.x == 0.0f && regroup.y == 0.0f)
    {
        return m_bot.GetStartLocation();
    }
    else
    {
        return regroup;
    }
}


const sc2::Unit * Squad::unitClosestToEnemy() const
{
    const sc2::Unit * closest = nullptr;
    float closestDist = std::numeric_limits<float>::max();

    for (auto unit : m_units)
    {
        BOT_ASSERT(unit, "null unit");
		
			// the distance to the order position
			int dist = m_bot.Map().getGroundDistance(unit->pos, m_order.getPosition());

			if (dist != -1 && (!closest || dist < closestDist))
			{
				closest = unit;
				closestDist = (float)dist;
			}
		
    }

    return closest;
}

int Squad::squadUnitsNear(const sc2::Point2D & p) const
{
    int numUnits = 0;

    for (auto unit : m_units)
    {
        BOT_ASSERT(unit, "null unit");

        if (Util::Dist(unit->pos, p) < 20.0f)
        {
            numUnits++;
        }
    }

    return numUnits;
}

const std::set<const sc2::Unit *> & Squad::getUnits() const
{
    return m_units;
}

const SquadOrder & Squad::getSquadOrder()	const
{
    return m_order;
}

void Squad::addUnit(const sc2::Unit * unit)
{
    m_units.insert(unit);
}

void Squad::removeUnit(const sc2::Unit * unit)
{
    m_units.erase(unit);
}

const std::string & Squad::getName() const
{
    return m_name;
}