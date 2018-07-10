# ppic(Palmprint ID Center)

## service

## optimize

### verification

Traversing the database to find the most similar palmprint is an extremely time-consuming operation.

Therefore, an algorithm needs to be designed to achieve similar palmprint search in less time while ensuring the accuracy.

#### `Multilevel Clusterings Search`

There are `N` records in the database. If we traverse the database, time complexity is `O(n)`.

Now let's group all data into `L1` clusterings. For every clustering, if it's records more than `x`, group all data into `L2` clusterings. Iterate this step until the minimum granularity of the clusterings data size is less than `x`. The time complexity is `O(L1+L2+...+Ln+x)`.

##### How to generate `Multilevel Clusterings`

##### How to search with `Multilevel Clusterings`
