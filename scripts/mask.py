
masks = {
        # In some mobile platforms, OS does not let
        # pinning to only one core. So we pin it to
        # one prime and one gold core.
        "prime": [
                    "90", # 1001 0000
                    "a0", # 1010 0000
                    "c0", # 1100 0000
        ], "gold": [
                    "10", # 0001 0000
                    "20", # 0010 0000
                    "40", # 0100 0000
        ], "silver": [
                    "01", # 0000 0001
                    "02", # 0000 0010
                    "04", # 0000 0100
                    "08", # 0000 1000
        ]}
mask_offset = 0
mask_base = 0

def init_mask():
    global mask_offset
    mask_offset = -1

def finish_mask(core):
    global mask_offset, mask_base
    if core == None:
        return None
    mask_base = (mask_offset + mask_base) % len(masks[core])

def get_mask(core):
    global mask_offset, mask_base
    if core == None:
        return ""
    mask_offset += 1
    assert mask_offset < len(masks[core]), f"Error: none of the masks for Core \"{core}\" worked!"
    return f"taskset {masks[core][(mask_offset + mask_base) % len(masks[core])]}"
