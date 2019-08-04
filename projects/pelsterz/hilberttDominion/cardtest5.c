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
  // arrays of all estates and not estates
  int cards[MAX_HAND];
  for(i = 0; i < MAX_HAND; i++){
    cards[i] = copper;
  }
  int deck[MAX_DECK];
  for(i = 0; i < MAX_DECK; i++){
    cards[i] = copper;
  }

  printf ("TESTING drawCard():\n");
  for (p = 0; p < numPlayer; p++)
  {
    for (handCount = 1; handCount <= maxHandCount; handCount++)
    {
      for (deckCount = 0; deckCount <= maxDeckCount; deckCount++)
      {
        printf("Test player %d with %d card(s) and %d card(s) in deck.\n", p, handCount, deckCount);
        memset(&G, 23, sizeof(struct gameState));   // clear the game state
        r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
        G.handCount[p] = handCount;                 // set the number of cards on hand
        G.deckCount[p] = deckCount;                 // set the number of cards on deck
        G.discardCount[p] = 5;

        // Check player receives another card
        #if (NOISY_TEST == 1)
          printf("Check player receives another card\n");
        #endif
        memcpy(G.hand[p], cards, sizeof(int) * handCount); // set all the cards to cards
        memcpy(G.deck[p], cards, sizeof(int) * handCount); // set all the cards to cards
        temp = G.handCount[p];
        drawCard(p, &G);
        count += asserttrue(G.handCount[p], temp + 1); // check if the card count is correct
        total++;

        if(deckCount > 1){
          // Check deck loses a card
          #if (NOISY_TEST == 1)
            printf("Check deck loses a card\n");
          #endif
          memcpy(G.hand[p], cards, sizeof(int) * handCount); // set all the cards to cards
          memcpy(G.deck[p], cards, sizeof(int) * handCount); // set all the cards to cards
          temp = G.deckCount[p];
          drawCard(p, &G);
          count += asserttrue(G.deckCount[p], temp - 1); // check if the card count is correct
          total++;
        } else {
          // Check deck is reshuffled
          #if (NOISY_TEST == 1)
            printf("Check deck is reshuffled\n");
          #endif
          memcpy(G.hand[p], cards, sizeof(int) * handCount); // set all the cards to cards
          memcpy(G.deck[p], cards, sizeof(int) * handCount); // set all the cards to cards
          G.discardCount[p] = 5;
          temp = G.discardCount[p];
          drawCard(p, &G);
          count += asserttrue(G.deckCount[p], temp - 1); // check if the card count is correct
          total++;
        }
      }
    }
  }

  printf("%d out of %d passed.\n", count, total);

  return 0;
}
