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
  int numPlayer = 5;
  int maxBonus = 10;
  int p, r, c, h = 0, handCount;
  int bonus = 0;
  int k[10] = {adventurer, council_room, feast, gardens, mine, remodel,
    smithy, village, baron, great_hall};
  struct gameState G;
  int maxHandCount = 5;

  printf ("TESTING endTurn():\n");
  for (p = 2; p < numPlayer; p++)
  {
      printf("%d test player(s).\n", p);
      memset(&G, 23, sizeof(struct gameState));   // clear the game state
      r = initializeGame(p, k, seed, &G); // initialize a new game

      // Check player is incremented
      #if (NOISY_TEST == 1)
        printf("Check player is incremented\n");
      #endif
      G.whoseTurn = 0;
      for (i = 0; i < p - 1; i++){
        temp = G.whoseTurn;
        endTurn(&G);
        count += asserttrue(G.whoseTurn, temp + 1); // check if the number of players is correct
        total++;
      }

      // Check player is incremented (wraps around)
      #if (NOISY_TEST == 1)
        printf("Check player is incremented (wraps around)\n");
      #endif
      G.whoseTurn = p - 1;
      endTurn(&G);
      count += asserttrue(G.whoseTurn, 0); // check if the number of players is correct
      total++;

      // Check initial actions is 1
      #if (NOISY_TEST == 1)
        printf("Check initial actions is 1\n");
      #endif
      endTurn(&G);
      count += asserttrue(G.numActions, 1); // check if the number of actions is correct
      total++;

      // Check initial buys is 1
      #if (NOISY_TEST == 1)
        printf("Check initial buys is 1\n");
      #endif
      endTurn(&G);
      count += asserttrue(G.numBuys, 1); // check if the number of buys is correct
      total++;
  }

  printf("%d out of %d passed.\n", count, total);

  return 0;
}
