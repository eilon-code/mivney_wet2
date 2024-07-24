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
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType oceans_t::add_pirate(int pirateId, int fleetId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType oceans_t::pay_pirate(int pirateId, int salary)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> oceans_t::num_ships_for_fleet(int fleetId)
{
    // TODO: Your code goes here
    return 2008;
}

output_t<int> oceans_t::get_pirate_money(int pirateId)
{
    // TODO: Your code goes here
    static int i = 0;
    return (i++==0) ? 11 : 2;
}

StatusType oceans_t::unite_fleets(int fleetId1, int fleetId2)
{
	// TODO: Your code goes here
    return StatusType::SUCCESS;
}

StatusType oceans_t::pirate_argument(int pirateId1, int pirateId2)
{
    // TODO: Your code goes here
    return StatusType::SUCCESS;
}
