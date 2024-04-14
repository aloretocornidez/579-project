/*

Psuedo-code for the findChildren() method
This code takes into account the various transporation methods and creates a unique child for each option
It also handles the logic of determining which transporation methods are valid and available
It does not handle weeding out potential loops

findChildren(Node node)
{

    children = Node[]

    nodeLocation = loc_.db_.find(node.locationId)

    for (path in nodeLocation.outbound_paths)
    {

        pathDestinationLocation = path_.db_b.find(path. whichever locationId we didn't start out from (src or dest))

        if (path supports walking)
        {
            if (node.arrivalMethod = walking)
            {
                create new node with location_id = pathDestinationLocation and arrivalMethod = walking
                calculate the cost using path.dist_mile and speed of walking
                append to children
            }
            else if (node.arrivalMethod == biking)
            {
                if (nodeLocation.isBikeDepot == true)
                {
                    create new node with location_id = pathDestinationLocation and arrivalMethod = walking
                    calculate the cost using path.dist_mile and speed of walking
                    append to children
                }
            }
            else if (node.arrivalMethod = cattran)
            {
                if (nodeLocation.isCatTranStop == true)
                {
                    create new node with location_id = pathDestinationLocation and arrivalMethod = walking
                    calculate the cost using path.dist_mile and speed of walking
                    append to children
                }
            }
        }
        if (path supports biking)
        {
            if (node.arrivalMethod == walking)
            {
                if (nodeLocation.isBikeDepot == true)
                {
                    create new node with location_id = pathDestinationLocation and arrivalMethod = biking
                    calculate the cost using path.dist_mile and speed of biking
                    append to children
                }
            }
            else if (node.arrivalMethod == biking)
            {
                create new node with location_id = pathDestinationLocation and arrivalMethod = biking
                calculate the cost using path.dist_mile and speed of biking
                append to children
            }
            else if (node.arrivalMethod == cattran)
            {
                if (nodeLocation.isCatTranStop == true && nodeLocation.isBikeDepot == true)
                {
                    create new node with location_id = pathDestinationLocation and arrivalMethod = biking
                    calculate the cost using path.dist_mile and speed of biking
                    append to children
                }
            }
        }
        if (path supports cattran) {
            if (node.arrivalMethod = walking)
            {
                if (nodeLocation.isCatTranStop == true) {
                    create new node with location_id = pathDestinationLocation and arrivalMethod = cattran
                    calculate the cost usig path.dist_mile and speed of cattran
                    append to children
                }
            }
            else if (node.arrivalMethod = biking)
            {
                if (nodeLocation.isBikeDepot == true && nodeLocation.isCatTranStop == true)
                {
                    create new node with location_id = pathDestinationLocation and arrivalMethod = cattran
                    calculate the cost usig path.dist_mile and speed of cattran
                    append to children
                }
            }
            else
            {
                create new node with location_id = pathDestinationLocation and arrivalMethod = cattran
                calculate the cost usig path.dist_mile and speed of cattran
                append to children
            }
        }
    }

    return children

}

*/