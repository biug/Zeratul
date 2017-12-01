#include "sc2api/sc2_api.h"

#include "Zeratul.h"
#include "Util.h"

Zeratul::Zeratul(const std::string & configFile)
    : m_map(*this)
    , m_bases(*this)
    , m_unitInfo(*this)
    , m_workers(*this)
    , m_gameCommander(*this)
    , m_strategy(*this)
    , m_techTree(*this)
	, m_state(*this)
	, m_config(configFile)
	, ConfigAgent(configFile)
{
    
}

void Zeratul::OnGameStart() 
{
    m_config.readConfigFile();
    
    // get my race
    auto playerID = Observation()->GetPlayerID();
    for (auto & playerInfo : Observation()->GetGameInfo().player_info)
    {
        if (playerInfo.player_id == playerID)
        {
            m_playerRace[Players::Self] = playerInfo.race_actual;
        }
        else
        {
            m_playerRace[Players::Enemy] = playerInfo.race_requested;
        }
    }
    
    m_techTree.onStart();
    m_strategy.onStart();
    m_map.onStart();
    m_unitInfo.onStart();
    m_bases.onStart();
    m_workers.onStart();

    m_gameCommander.onStart();
}

void Zeratul::OnStep()
{
    Control()->GetObservation();

    m_map.onFrame();
    m_unitInfo.onFrame();
    m_bases.onFrame();
    m_workers.onFrame();
    m_strategy.onFrame();
	m_state.onFrame();

    m_gameCommander.onFrame();

    Debug()->SendDebug();
}

void Zeratul::OnUpgradeCompleted(sc2::UpgradeID upgradeID)
{
	m_state.OnUpgradeCompleted(upgradeID);
}

// TODO: Figure out my race
const sc2::Race & Zeratul::GetPlayerRace(int player) const
{
    BOT_ASSERT(player == Players::Self || player == Players::Enemy, "invalid player for GetPlayerRace");
    return m_playerRace[player];
}

BotConfig & Zeratul::Config()
{
     return m_config;
}

const MapTools & Zeratul::Map() const
{
    return m_map;
}

const StrategyManager & Zeratul::Strategy() const
{
    return m_strategy;
}

const BaseLocationManager & Zeratul::Bases() const
{
    return m_bases;
}

const UnitInfoManager & Zeratul::UnitInfo() const
{
    return m_unitInfo;
}

const TypeData & Zeratul::Data(const sc2::UnitTypeID & type) const
{
    return m_techTree.getData(type);
}

const TypeData & Zeratul::Data(const sc2::UpgradeID & type) const
{
    return m_techTree.getData(type);
}

const TypeData & Zeratul::Data(const BuildType & type) const
{
    return m_techTree.getData(type);
}

WorkerManager & Zeratul::Workers()
{
    return m_workers;
}

const sc2::Unit * Zeratul::GetUnit(const UnitTag & tag) const
{
    return Observation()->GetUnit(tag);
}

StateManager & Zeratul::State()
{
	return m_state;
}

sc2::Point2D Zeratul::GetStartLocation() const
{
    return Observation()->GetStartLocation();
}

void Zeratul::OnError(const std::vector<sc2::ClientError> & client_errors, const std::vector<std::string> & protocol_errors)
{
    
}