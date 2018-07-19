# ppic(Palmprint ID Center)

[![pipeline status](https://gitlab.com/leosocy/ppic/badges/master/pipeline.svg)](https://gitlab.com/leosocy/ppic/commits/master)
[![codecov](https://codecov.io/gh/PalmID/ppic/branch/master/graph/badge.svg)](https://codecov.io/gh/PalmID/ppic)
[![MIT licensed](https://img.shields.io/badge/license-MIT-green.svg)](https://raw.githubusercontent.com/PalmID/ppic/master/LICENSE)

## service

## optimize

### verification

Traversing the database to find the most similar palmprint is an extremely time-consuming operation.

Therefore, an algorithm needs to be designed to achieve similar palmprint search in less time while ensuring the accuracy.

#### `Multilevel Clusterings Search`

There are `N` records in the database. If we traverse the database, time complexity is `O(n)`.

Now let's group all data into `L1` clusterings. For every clustering, if it's records more than `x`, group all data into `L2` clusterings. Iterate this step until the minimum granularity of the clusterings data size is less than `x`. The time complexity is `O(L1+L2+...+Ln+x)`.

##### How to generate `Multilevel Clusterings`

##### How to search with `Multilevel Clusterings`
