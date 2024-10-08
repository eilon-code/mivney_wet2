#include "pirates24b2.h"

int getPirateRank(Pirate* pirate, UpTree<Pirate, Fleet>::Set** set);

oceans_t::oceans_t()=default;

oceans_t::~oceans_t()=default;

StatusType oceans_t::add_fleet(int fleetId)
{
	if (fleetId <= 0) return StatusType::INVALID_INPUT;
	Fleet fleet(fleetId);
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
    return set->id.getShipCount();
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
	return m_unionFind.union2Sets(fleetId1, fleetId2);
}

StatusType oceans_t::pirate_argument(int pirateId1, int pirateId2)
{
	if (pirateId1 <= 0 || pirateId2 <= 0 || pirateId1 == pirateId2) return StatusType::INVALID_INPUT;
	output_t<Pirate*> searchPirate1 = m_unionFind.fetch(pirateId1);
	if (searchPirate1.status() != StatusType::SUCCESS) return searchPirate1.status();
	Pirate* pirate1 = searchPirate1.ans();
	// std::cout << "pirateId1=" << pirate1->getId() << ", base_rank: "<< pirate1->getRank() << std::endl;

	output_t<Pirate*> searchPirate2 = m_unionFind.fetch(pirateId2);
	if (searchPirate2.status() != StatusType::SUCCESS) return searchPirate2.status();
	Pirate* pirate2 = searchPirate2.ans();
	// std::cout << "pirateId2=" << pirate2->getId() << ", base_rank: "<< pirate2->getRank() << std::endl;

	output_t<UpTree<Pirate, Fleet>::Set*> search1 = m_unionFind.fetchSetOf(pirateId1);
	if (search1.status() != StatusType::SUCCESS) return search1.status();
	UpTree<Pirate, Fleet>::Set* set1 = search1.ans();

	output_t<UpTree<Pirate, Fleet>::Set*> search2 = m_unionFind.fetchSetOf(pirateId2);
	if (search2.status() != StatusType::SUCCESS) return search2.status();
	UpTree<Pirate, Fleet>::Set* set2 = search2.ans();

    int rank1 = getPirateRank(pirate1, &set1);
	int rank2 = getPirateRank(pirate2, &set2);
	// std::cout << "Ranks: " << "pirate" << pirateId1 << ": " << rank1 << ", pirate" << pirateId2 << ": " << rank2 << std::endl;
	
	bool sameFleet = (set1 == set2);
	if (!sameFleet) return StatusType::FAILURE;
	
	pirate1->changeCoins(rank2 - rank1);
	pirate2->changeCoins(rank1 - rank2);
	return StatusType::SUCCESS;
}

int getPirateRank(Pirate* pirate, UpTree<Pirate, Fleet>::Set** set) {
	UpTree<Pirate, Fleet>::Set* subSet = *set;
	int totalRankOffset = 0;
	// std::cout << "rankFleet: " << set->id.rankOffset() << std::endl;
	while (!(*set)->isRoot()) {
		totalRankOffset += (*set)->id.rankOffset();
		*set = (*set)->superSet;
		// std::cout << "rankFleet: " << set->id.rankOffset() << std::endl;
	}
	totalRankOffset += (*set)->id.rankOffset();
	int result = pirate->getRank() + totalRankOffset;
	totalRankOffset -= (*set)->id.rankOffset();
	// std::cout << "rankResult: " << result << std::endl;

	while (subSet->superSet && subSet->superSet != *set) { // cutting down the tree
        totalRankOffset -= subSet->id.rankOffset();

        UpTree<Pirate, Fleet>::Set* temp = subSet->superSet;
        subSet->superSet = *set;
        subSet->id.offsetRank(totalRankOffset);
        subSet = temp;
    }
	
	return result;
}

output_t<int> oceans_t::get_pirate_rank(int pirateId)
{
    if (pirateId <= 0) return StatusType::INVALID_INPUT;
	output_t<Pirate*> searchPirate = m_unionFind.fetch(pirateId);
	if (searchPirate.status() != StatusType::SUCCESS) return searchPirate.status();
	Pirate* pirate = searchPirate.ans();

	output_t<UpTree<Pirate, Fleet>::Set*> search = m_unionFind.fetchSetOf(pirateId);
	if (search.status() != StatusType::SUCCESS) return search.status();
	UpTree<Pirate, Fleet>::Set* set = search.ans();

	return getPirateRank(pirate, &set);
}
