


def find_not_match(timetree1, timetree2, t1, t2, K):
    """
    For the two timetrees, find the at most K bins
    where they do not match that are in the region
    t1, t2. Substantially faster if the regions
    are on bin boundaries.
    
    """

    tt1 = timetree1
    tt2 = timetree2

    out = []



    
    # treat  tt1 as the canonical bin
    ML = tt1.maxlevel


    def get_not_match(level, bins):
        if len(out) > K:
            return

        for bi in bins:
            bin_t1, bin_t2 = tt1.get_bin_range(level, bi)

            if bin_t1 <= t2 and bin_t2 >= t1:
                # This is a valid time range, check if hashes mastch
                h1 = tt1.get_hash(level, bi)
                h2 = tt2.get_hash(level, bi)
                if h1 != h2:
                    if level == 0:
                        out.append(bi)
                    else:
                        get_not_match(level - 1, tt1.get_sub_bins(level, bi))
            else:
                # 
                pass
            
    b1 = tt1.get_bin(ML, t1)
    b2 = tt1.get_bin(ML, t2)

    get_not_match(ML, range(b1, b2+1))

    return out

