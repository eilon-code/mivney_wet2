#include "pirates24b2.h"

oceans_t::oceans_t()
{
	// TODO: Your code goes here
}

oceans_t::~oceans_t()
{
	// TODO: Your code goes here
}


StatusType oceans_t::add_fleet(int fleetId)
{
	if (fleetId <= 0) return StatusType::INVALID_INPUT;
	Fleet fleet = Fleet(fleetId);
	return m_unionFind.makeSet(fleet);
}

StatusType oceans_t::add_pirate(int pirateId, int fleetId)
{
	if (fleetId <= 0 || pirateId <= 0) return StatusType::INVALID_INPUT;
	Pirate pirate(pirateId);
	return m_unionFind.insertValue(fleetId, pirate);
}

StatusType oceans_t::pay_pirate(int pirateId, int salary)
{
	if (pirateId <= 0 || salary <= 0) return StatusType::INVALID_INPUT;
	output_t<Pirate*> search = m_unionFind.fetch(pirateId);
	if (search.status() != StatusType::SUCCESS) return search.status();
	Pirate* pirate = search.ans();
	pirate->changeCoins(salary);
	return StatusType::SUCCESS;
}

output_t<int> oceans_t::num_ships_for_fleet(int fleetId)
{
	if (fleetId <= 0) return StatusType::INVALID_INPUT;
	output_t<UpTree<Pirate, Fleet>::Set*> search = m_unionFind.findSet(fleetId);
	if (search.status() != StatusType::SUCCESS) return search.status();
	UpTree<Pirate, Fleet>::Set* set = search.ans();
    return set->id.ships();
}

output_t<int> oceans_t::get_pirate_money(int pirateId)
{
	if (pirateId <= 0) return StatusType::INVALID_INPUT;
    output_t<Pirate*> search = m_unionFind.fetch(pirateId);
	if (search.status() != StatusType::SUCCESS) return search.status();
	Pirate* pirate = search.ans();
	return pirate->getCoins();
}

StatusType oceans_t::unite_fleets(int fleetId1, int fleetId2)
{
	if (fleetId1 <= 0 || fleetId2 <= 0 || fleetId1 == fleetId2) return StatusType::INVALID_INPUT;
	return m_unionFind.union2Set(fleetId1, fleetId2);
}

StatusType oceans_t::pirate_argument(int pirateId1, int pirateId2)
{
	if (pirateId1 <= 0 || pirateId2 <= 0 || pirateId1 == pirateId2) return StatusType::INVALID_INPUT;
	output_t<Pirate*> searchPirate1 = m_unionFind.fetch(pirateId1);
	if (searchPirate1.status() != StatusType::SUCCESS) return searchPirate1.status();
	Pirate* pirate1 = searchPirate1.ans();

	output_t<Pirate*> searchPirate2 = m_unionFind.fetch(pirateId2);
	if (searchPirate2.status() != StatusType::SUCCESS) return searchPirate2.status();
	Pirate* pirate2 = searchPirate2.ans();

	output_t<UpTree<Pirate, Fleet>::Set*> search1 = m_unionFind.findSetOf(pirate1->getId());
	if (search1.status() != StatusType::SUCCESS) return search1.status();
	UpTree<Pirate, Fleet>::Set* set1 = search1.ans();

	output_t<UpTree<Pirate, Fleet>::Set*> search2 = m_unionFind.findSetOf(pirate2->getId());
	if (search2.status() != StatusType::SUCCESS) return search2.status();
	UpTree<Pirate, Fleet>::Set* set2 = search2.ans();

	if (set1 != set2) return StatusType::FAILURE;

    int rank1 = getPirateRank(pirate1, set1);
	int rank2 = getPirateRank(pirate2, set2);
	
	pirate1->changeCoins(rank1 - rank2);
	pirate2->changeCoins(rank2 - rank1);
}

int getPirateRank(Pirate* pirate, UpTree<Pirate, Fleet>::Set* set) {
	int result = pirate->getRank();
	while (set != nullptr) {
		result += set->id.rankOffset();
		set = set->superSet;
	}
	return result;
}
