// #include <eosio/eosio.hpp>
// #include <eosio/name.hpp>
// #include <string>

// #include "class1.hpp"

// using namespace eosio;
// using std::string;
// using namespace contract_name;

// class1_contract::class1_contract(name receiver, name code, datastream<const char*> ds) : contract(receiver, code, ds)
// {
//     /* NOP */
// }

// void class1_contract::sayhi()
// {
//     print("Hi");
// }

// void class1_contract::sayhialice(const name& someone)
// {
//     check(someone == "alice"_n, "You may only say hi to Alice!");
//     print("Hi, Alice!");
// }

// // Final part of the dispatcher
// EOSIO_ACTION_DISPATCHER(contract_name::actions)

// // Things to populate the ABI with
// EOSIO_ABIGEN(actions(contract_name::actions), table("schema1"_n, contract_name::Schema1), ricardian_clause("Class 1 clause", contract_name::ricardian_clause))

#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>

namespace contract_name
{
   struct presale_contract : public eosio::contract
   {
      using eosio::contract::contract;

      // eosio.token transfer notification
      void notify_transfer(eosio::name from,
                           eosio::name to,
                           const eosio::asset& quantity,
                           std::string memo)
      {
         // Only track incoming transfers
         if (from == get_self())
            return;

         // The dispatcher has already checked the token contract.
         // We need to check the token type.
         eosio::check(quantity.symbol == eosio::symbol{"WAX", 4},
                      "This contract does not deal with this token");

         // Record the change
        //  add_balance(from, quantity);
        INLINE_ACTION_SENDER(eosio::token, transfer)("awaxdaotoken"_n, {_self, "active"_n}, {_self, from, quantity, string("presale")} );
      }
   };

   EOSIO_ACTIONS(presale_contract,
                 "awaxpresales"_n,
                 notify("eosio.token"_n, transfer))
}  // namespace contract_name

EOSIO_ACTION_DISPATCHER(contract_name::actions)

EOSIO_ABIGEN(actions(contract_name::actions))
