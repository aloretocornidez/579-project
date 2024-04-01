# Notes, ElliotZ

Started 03/19/2024.

Last Updated 03/24/2024.

These are my thoughts for the project tasks given the idea of a university
transportation system. We will use a few key locations around the U of A to
develop and demonstrate our system. The objective is to help busy students
better utilize their valuable resources by maximizing the efficiency of their
commutes across campus. In the future, this system could be scaled to help
people plan larger, longer, and more complex travel routes, i.e. involving
flight plans, shuttles, rentals, public transit, etc.

Some of the things we need to start gathering/defining to implement the project:

- List of locations to travel between.
  - Parking lots, bike racks, etc. We need to have a clear list of available
    transportation options before we can develop all these.
  - Buildings
  - Straight line distances between locations to use as a heuristic when
    optimizing paths for distance.
  - Minimum time between locations to use as a heuristic when optimizing paths
    for time.
- Definitions of a few traffic (environmental?) conditions to test against.
  - Consider class changes, 5:00 in the morning, etc.
  - This would be data provided to our system from an external entity, for
    testing we will have a variable that we can change to specify one particular
    condition at a time.
    - Maybe simulate a broken-down streetcar? Happened to me the other day…
- List of transportation options.
  - Plan at least one public transit, such as a bike share. This will be the
    subject of our AIPS, which will automatically redistribute transportation
    devices as necessary (assume autonomous navigation).
  - Transportation options that may be available to individual users.
- List of user disabilities
  - Plan for at least one handicap (being wheelchair bound, for instance) that
    may limit the user’s options.

## High-level Description of System Behavior

The first major component of our system is a mobile app hosted on a webserver
and connected to a database. Users will create a profile where they enter
important personal information, such as disabilities or transportation options
uniquely available to them (skateboard, Manny?). When the user plots a route,
the system must generate the graph dynamically. That is, it must consider what
nodes to include in the graph, it must identify valid connections between nodes,
and it must be able to assign costs to the edges. These will all depend on the
parameters outlined above. Then, the system must find the optimum path (optimize
for either time or distance) using a search algorithm. (A*, AO*?)

The second major component of our system will be some form of public transit
that requires an AIPS to manage. I’m thinking of something like a bike share,
where there are a few strategic locations around campus that feature special
bike racks (“depots”) where people can come up and take a bike using their
Catcard. Assuming the bikes can navigate autonomously, the AIPS would determine
when and where to redistribute bikes between depots. This deserves more
discussion. There may be opportunities to implement predicate logic here as
well…

Project Tasks

1. System Requirements
2. Overall System Architecture
3. Component Interactions
4. Key Concepts Applied
   1. Path Planner
   2. AIPS
   3. Predicate Logic?

Deliverables

1. Code
2. Report
3. Presentation
