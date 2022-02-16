#include <string>
#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>

#include "token.hpp"

using namespace std;
using namespace eosio;
using namespace token;

#define WAX_SYMBOL symbol("WAX", 4)
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

         if (memo == "deposit") return;

         // The dispatcher has already checked the token contract.
         // We need to check the token type.
         // eosio::check(quantity.symbol == eosio::symbol{"WAX", 4},
         //              "Only WAX tokens are accepted for presale");

         eosio::check(quantity.symbol.code() == WAX_SYMBOL.code(), "only WAX token is accepted");
         
         asset payout = quantity;
         payout.amount = payout.amount / 10000;
         payout.symbol = symbol("AWAX", 4);

         // Transfer back AWAX tokens
         INLINE_ACTION_SENDER(token_contract, transfer)("awaxdaotoken"_n, {get_self(), "active"_n}, {get_self(), from, payout, string("presale")} );
      }
   };

   EOSIO_ACTIONS(presale_contract,
                 "awaxpresales"_n,
                 notify("eosio.token"_n, transfer))
}  // namespace contract_name

EOSIO_ACTION_DISPATCHER(contract_name::actions)

EOSIO_ABIGEN(actions(contract_name::actions))
