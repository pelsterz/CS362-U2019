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
  int estates[MAX_HAND];
  int curses[MAX_HAND];
  for(i = 0; i < MAX_HAND; i++){
    estates[i] = estate;
    curses[i] = curse;
  }
  int players[MAX_PLAYERS];

  printf ("TESTING getWinners():\n");
  for (p = 0; p < numPlayer; p++)
  {
    for (handCount = 1; handCount <= maxHandCount; handCount++)
    {
      printf("Test player %d with %d card(s) and %d bonus.\n", p, handCount, bonus);
      memset(&G, 23, sizeof(struct gameState));   // clear the game state
      r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
      G.handCount[p] = handCount;                 // set the number of cards on hand

      // Check player has highscore
      #if (NOISY_TEST == 1)
        printf("Check player has highscore\n");
      #endif
      for (i = 0; i < numPlayer; i++){
        if (i != p) memcpy(G.hand[i], curses, sizeof(int) * handCount); // set all the cards to curses
      }
      memcpy(G.hand[p], estates, sizeof(int) * handCount); // set all the cards to estates
      getWinners(players, &G);
      count += asserttrue(players[p], 1); // check if the score is correct
      total++;

      // Check player does not have highscore
      #if (NOISY_TEST == 1)
        printf("Check player does not have highscore\n");
      #endif
      for (i = 0; i < numPlayer; i++){
        if (i != p) memcpy(G.hand[i], estates, sizeof(int) * handCount); // set all the cards to estates
      }
      memcpy(G.hand[p], curses, sizeof(int) * handCount); // set all the cards to curses
      getWinners(players, &G);
      count += asserttrue(players[p], 0); // check if the score is correct
      total++;
    }
  }

  printf("%d out of %d passed.\n", count, total);

  return 0;
}
