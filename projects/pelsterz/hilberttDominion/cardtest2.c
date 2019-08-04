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
  int p, r, c, h = 0, handCount, deckCount;
  int bonus = 0;
  int k[10] = {adventurer, council_room, feast, gardens, mine, remodel,
    smithy, village, baron, great_hall};
  struct gameState G;
  int maxHandCount = 5;
  int maxDeckCount = 5;

  printf ("TESTING shuffle():\n");
  for (p = 0; p < numPlayer; p++)
  {
    for (deckCount = 0; deckCount <= maxDeckCount; deckCount++)
    {
      printf("Test player %d with %d card(s) in deck and %d bonus.\n", p, deckCount, bonus);
      memset(&G, 23, sizeof(struct gameState));   // clear the game state
      r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
      G.deckCount[p] = deckCount;                 // set the number of cards on deck

      // Check deck count has not changed
      #if (NOISY_TEST == 1)
        printf("Check deck count has not changed\n");
      #endif
      temp = G.deckCount[p];
      shuffle(p, &G);
      count += asserttrue(G.deckCount[p], temp); // check if the number of cards in the deck is correct
      total++;
    }
  }

  printf("%d out of %d passed.\n", count, total);

  return 0;
}
