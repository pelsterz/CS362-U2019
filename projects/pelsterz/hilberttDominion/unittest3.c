#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

// set NOISY_TEST to 0 to remove printfs describing tests from output
#define NOISY_TEST 0

// Custom assert function (returns 1 if true, allows number of successes to be counted)
// a = Actual, b = Expected
int asserttrue(int a, int b){
  printf("Actual: %d, Expected: %d\n", a, b);
  if(a == b) return 1;
  else return 0;
}

int main() {
  int i, temp = 0, total = 0, count = 0;
  int seed = 1000;
  int numPlayer = 2;
  int maxBonus = 10;
  int p, r, c, h = 0, handCount;
  int bonus = 0;
  int k[10] = {adventurer, council_room, feast, gardens, mine, remodel,
    smithy, village, baron, great_hall};
  struct gameState G;
  int maxHandCount = 5;
  // arrays of all estates and not estates
  int cards[MAX_HAND];
  for(i = 0; i < MAX_HAND; i++){
    cards[i] = copper;
  }

  printf ("TESTING _ambassador_effect():\n");
  for (p = 0; p < numPlayer - 1; p++)
  {
    for (handCount = 1; handCount <= maxHandCount; handCount++)
    {
      printf("Test player %d with %d card(s) and %d bonus.\n", p, handCount, bonus);
      memset(&G, 23, sizeof(struct gameState));   // clear the game state
      r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
      G.handCount[p] = handCount;                 // set the number of cards on hand

      // Ensure player returns 2 or less copies
      #if (NOISY_TEST == 1)
        printf("Ensure player returns 2 or less copies\n");
      #endif
      memcpy(G.hand[p], cards, sizeof(int) * handCount); // set all the cards to copper
      temp = G.handCount[p];
      int choice[] = {1};
      if(G.handCount[p] >= 2){ // Greater than 2 cards in hand
        _ambassador_effect(choice, 1, &G, 2);
        count += asserttrue(G.handCount[p], temp - 2); // check if the number of cards is correct
      } else if(G.handCount[p] == 1){ // One card in hand
        _ambassador_effect(choice, 1, &G, 1);
        count += asserttrue(G.handCount[p], temp - 1); // check if the number of cards is correct
      } else { // Zero cards in hand
        _ambassador_effect(choice, 1, &G, 0);
        count += asserttrue(G.handCount[p], temp); // check if the number of cards is correct
      }
      total++;

      // Ensure supply increases by appropriate amount
      #if (NOISY_TEST == 1)
        printf("Ensure player returns 2 or less copies\n");
      #endif
      memcpy(G.hand[p], cards, sizeof(int) * handCount); // set all the cards to copper
      temp = G.supplyCount[p];
      choice[0] = 1;
      if(G.handCount[p] >= 2){ // Greater than 2 cards in hand
        _ambassador_effect(choice, 1, &G, 2);
        count += asserttrue(G.supplyCount[p], temp + 2); // check if the number of cards is correct
      } else if(G.handCount[p] == 1){ // One card in hand
        _ambassador_effect(choice, 1, &G, 1);
        count += asserttrue(G.supplyCount[p], temp + 1); // check if the number of cards is correct
      } else { // Zero cards in hand
        _ambassador_effect(choice, 1, &G, 0);
        count += asserttrue(G.supplyCount[p], temp); // check if the number of cards is correct
      }
      total++;

      // Check that coins have not changed
      #if (NOISY_TEST == 1)
        printf("Check that coins have not changed\n");
      #endif
      memcpy(G.hand[p], cards, sizeof(int) * handCount); // set all the cards to copper
      temp = G.coins;
      choice[0] = 1;
      _ambassador_effect(choice, 1, &G, 0);
      count += asserttrue(G.coins, temp); // check if the number of coins is correct
      total++;

      // Check that each other player has an additional card
      #if (NOISY_TEST == 1)
        printf("Check that each other player has an additional card\n");
      #endif
      memcpy(G.hand[p], cards, sizeof(int) * handCount); // set all the cards to silver
      choice[0] = 1;
      for(i = 0; i < numPlayer; i++){
        if(i != p){
          temp = G.handCount[i];
          _ambassador_effect(choice, 1, &G, 1); // choice 2 (discard hand, +4 cards)
          count += asserttrue(G.handCount[i], temp + 1); // check if the number of cards is correct
          total++;
        }
      }
    }
  }

  printf("%d out of %d passed.\n", count, total);

  return 0;
}
