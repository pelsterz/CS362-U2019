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
  int p, r, c, h, handCount;
  int bonus = 0;
  int k[10] = {adventurer, council_room, feast, gardens, mine, remodel,
    smithy, village, baron, great_hall};
  struct gameState G;
  int maxHandCount = 5;
  // arrays of all estates and not estates
  int estates[MAX_HAND];
  int not_estates[MAX_HAND];
  for(i = 0; i < MAX_HAND; i++){
    estates[i] = estate;
    not_estates[i] = copper;
  }

  printf ("TESTING baronCard():\n");
  for (p = 0; p < numPlayer; p++)
  {
    for (handCount = 1; handCount <= maxHandCount; handCount++)
    {
      printf("Test player %d with %d card(s) and %d bonus.\n", p, handCount, bonus);
      memset(&G, 23, sizeof(struct gameState));   // clear the game state
      r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
      G.handCount[p] = handCount;                 // set the number of cards on hand

      // Ensure G.numBuys gets a +1
      #if (NOISY_TEST == 1)
        printf("Ensure G.numBuys gets a +1\n");
      #endif
      memcpy(G.hand[p], estates, sizeof(int) * handCount); // set all the cards to copper
      temp = G.numBuys;
      int choice[] = {0};
      _baron_effect(choice, 1, &G); // choice 0 (given estate card)
      count += asserttrue(G.numBuys, temp + 1); // check if the number of buys is correct
      total++;

      // Check that estate card is given
      #if (NOISY_TEST == 1)
        printf("Check that estate card is given\n");
      #endif
      memcpy(G.hand[p], estates, sizeof(int) * handCount); // set all the cards to copper
      temp = G.handCount[p];
      choice[0] = 0;
      _baron_effect(choice, 1, &G); // choice 0 (given estate card)
      count += asserttrue(G.handCount[p], temp + 1); // check if the number of cards is correct
      total++;

      // Check that G.coins gets +4
      #if (NOISY_TEST == 1)
        printf("Check that G.coins gets +4\n");
      #endif
      memcpy(G.hand[p], estates, sizeof(int) * handCount); // set all the cards to silver
      temp = G.coins;
      choice[0] = 1;
      _baron_effect(choice, 1, &G); // choice 1 (try to discard estate)
      count += asserttrue(G.coins, temp + 4); // check if the number of coins is correct
      total++;

      // Check that estate card is taken away
      #if (NOISY_TEST == 1)
        printf("Check that estate card is taken away\n");
      #endif
      memcpy(G.hand[p], estates, sizeof(int) * handCount); // set all the cards to silver
      temp = G.handCount[p];
      choice[0] = 1;
      _baron_effect(choice, 1, &G); // choice 1 (try to discard estate)
      count += asserttrue(G.handCount[p], temp - 1); // check if the number of coins is correct
      total++;

      // Check that estate card is given when no estate cards in hand
      #if (NOISY_TEST == 1)
        printf("Check that estate card is given when no estate cards in hand\n");
      #endif
      memcpy(G.hand[p], not_estates, sizeof(int) * handCount); // set all the cards to gold
      temp = G.handCount[p];
      choice[0] = 1;
      _baron_effect(choice, 1, &G); // choice 1 (try to discard estate)
      count += asserttrue(G.handCount[p], temp + 1); // check if the number of cards is correct
      total++;
    }
  }

  printf("%d out of %d passed.\n", count, total);

  return 0;
}
