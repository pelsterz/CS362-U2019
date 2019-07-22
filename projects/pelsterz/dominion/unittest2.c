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
  int cards[MAX_HAND];
  for(i = 0; i < MAX_HAND; i++){
    cards[i] = copper;
  }

  printf ("TESTING minionCard():\n");
  p = 0;
  for (handCount = 1; handCount <= maxHandCount; handCount++)
  {
    printf("Test player %d with %d card(s) and %d bonus.\n", p, handCount, bonus);
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[p] = handCount;                 // set the number of cards on hand

    // Ensure G.numActions gets a +1
    #if (NOISY_TEST == 1)
      printf("Ensure G.numActions gets a +1\n");
    #endif
    memcpy(G.hand[p], cards, sizeof(int) * handCount); // set all the cards to copper
    temp = G.numActions;
    minionCard(c, 1, 0, 0, &G, h, &bonus, p); // choice 1 (+2 coins)
    count += asserttrue(G.numActions, temp + 1); // check if the number of actions is correct
    total++;

    // Check that choice 1 gives +2 coins
    #if (NOISY_TEST == 1)
      printf("Check that choice 1 gives +2 coins\n");
    #endif
    memcpy(G.hand[p], cards, sizeof(int) * handCount); // set all the cards to copper
    temp = G.coins;
    minionCard(c, 1, 0, 0, &G, h, &bonus, p); // choice 1 (+2 coins)
    count += asserttrue(G.coins, temp + 2); // check if the number of coins is correct
    total++;

    // Check that hand is now 4 cards
    #if (NOISY_TEST == 1)
      printf("Check that hand is now 4 cards\n");
    #endif
    memcpy(G.hand[p], cards, sizeof(int) * handCount); // set all the cards to silver
    minionCard(c, 0, 1, 0, &G, h, &bonus, p); // choice 2 (discard hand, +4 cards)
    count += asserttrue(G.handCount[p], 4); // check if the number of cards is correct
    total++;

    // Check that other players hands have not changed (or are now 4 cards)
    #if (NOISY_TEST == 1)
      printf("Check that other players hands have not changed (or are now 4 cards)\n");
    #endif
    memcpy(G.hand[p], cards, sizeof(int) * handCount); // set all the cards to silver
    if(p == 0){
      if(G.handCount[1] >= 5){
        minionCard(c, 0, 1, 0, &G, h, &bonus, p); // choice 2 (discard hand, +4 cards)
        count += asserttrue(G.handCount[1], 4); // check if the number of cards is correct
      } else {
        temp = G.handCount[1];
        minionCard(c, 0, 1, 0, &G, h, &bonus, p); // choice 2 (discard hand, +4 cards)
        count += asserttrue(G.handCount[1], temp); // check if the number of cards is correct
      }
    }
    else if(p == 1){
      if(G.handCount[0] >= 5){
        minionCard(c, 0, 1, 0, &G, h, &bonus, p); // choice 2 (discard hand, +4 cards)
        count += asserttrue(G.handCount[0], 4); // check if the number of cards is correct
      } else {
        temp = G.handCount[0];
        minionCard(c, 0, 1, 0, &G, h, &bonus, p); // choice 2 (discard hand, +4 cards)
        count += asserttrue(G.handCount[0], temp); // check if the number of cards is correct
      }
    }
    total++;
  }

  printf("%d out of %d passed.\n", count, total);

  return 0;
}
