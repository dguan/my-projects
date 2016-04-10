# partition seq_len into num_parts partitions, with min and max length
# Of course, seq_len MUST >= num_parts * min_len, and MUST <= num_parts * max_len
def part_into_parts_and_range(len, num_parts, min_len):
    results = []
    if len < num_parts*min_len:
        return results
    if num_parts == 1:
        results.append([len])
        return results
    max_len = len - (num_parts-1)*min_len + 1
    for i in range(min_len, max_len):
        temp = part_into_parts_and_range(len-i, num_parts-1, min_len)
        for x in temp:
            x.append(i)
            results.append(x)
    return results


if __name__ == '__main__' :
    while True:
        input_str = raw_input('Please input len, num_part, and min_len(CR to quit): ')
        if input_str=='' :
            break
        nums = map(int, input_str.split(' '))
        results = part_into_parts_and_range(nums[0], nums[1], nums[2])
        print results
        print 'Altogether {0} partitions.'.format(len(results))
        
    print 'Thanks for using, bye!'



