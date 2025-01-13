
<h1 align="center">Havok Toolset</h1>

<p align="center">
    Havok Toolset is a collection of conversion tools under HavokLib.
</p>

<p align="center">
    This toolset runs on Spike foundation.
</p>

<p align="center">
    Head to this
    <b><a href="https://github.com/PredatorCZ/Spike/wiki/Spike">Wiki</a></b>
    for more information on how to effectively use it.
</p>

<p align="center">
<b><a href="https://github.com/PredatorCZ/RevilLib/releases">Latest Release</a></b>
</p><h2>Module list</h2>
<ul>
<li><a href="#Havok2GLTF">Havok2GLTF</a></li>
</ul>

## Havok2GLTF

### Module command: hk_to_gltf

Converts Havok data into GLTF.

Supports animation and skeleton data.

### Input file patterns: `.hkx$`, `.hka$` or user specified

### Settings

- **extension-patterns**

  **CLI Long:** ***--extension-patterns***\
  **CLI Short:** ***-p***

  Specify extension patterns for file detecting separated by comma.

- **animation**

  - **blend-override**

    **CLI Long:** ***--blend-override***\
    **CLI Short:** ***-b***

    **Default value:** AUTO

    **Valid values:** AUTO, NORMAL, ADDITIVE_DEPRECATED, ADDITIVE

    Override blend hint.

  - **sample-rate**

    **CLI Long:** ***--sample-rate***\
    **CLI Short:** ***-r***

    **Default value:** 60

    Specify frames per second for GLTF.

  - **scale-type**

    **CLI Long:** ***--scale-type***\
    **CLI Short:** ***-S***

    **Default value:** NONE

    **Valid values:**

    - NONE: Ignore scale tracks.

    - INDEPENDENT:
    Input scale tracks does not affect child nodes positions.
    They also doesn't introduce shearing like in MATRICIAL case.

    Select desired processing mode for node scale tracks.

  - **filename-anims**

    **CLI Long:** ***--filename-anims***\
    **CLI Short:** ***-f***

    **Default value:** false

    Use filename for unnamed animations instead of Motion[]

- **skeleton**

  - **skeleton-path**

    **CLI Long:** ***--skeleton-path***\
    **CLI Short:** ***-s***

    Specify path to a skeleton hkx.

  - **generation**

    **CLI Long:** ***--generation***\
    **CLI Short:** ***-G***

    **Default value:** DEFAULT

    **Valid values:**

    - DEFAULT: Do not modify skeleton in any way.

    - AUTO: Automatically detect nodes with children where scale animation might occur.

    - MANUAL: Manually select bones (in gen-cbones) where scale motion occurs and have child nodes.

    Set additional skeleton modifications.

  - **gen-cbones**

    **CLI Long:** ***--gen-cbones***\
    **CLI Short:** ***-B***

    Comma separated bone names where should be applied scale tracks.

  - **visualize**

    **CLI Long:** ***--visualize***\
    **CLI Short:** ***-V***

    **Default value:** true

    Create visualization mesh for skeletons. (Enforces armature object for Blender)

  - **create-root-node**

    **CLI Long:** ***--create-root-node***\
    **CLI Short:** ***-N***

    **Default value:** false

    Force create root node named after skeleton. (Separates root motion)

- **scene**

  - **units**

    **CLI Long:** ***--units***\
    **CLI Short:** ***-u***

    **Default value:** METER

    **Valid values:** MM, CM, DM, METER, KM, INCH, FEET, MILE, CUSTOM

    Input scene units.

  - **custom-scale**

    **CLI Long:** ***--custom-scale***\
    **CLI Short:** ***-C***

    **Default value:** 1

    Set unit scale in case of CUSTOM units.

  - **up-axis**

    **CLI Long:** ***--up-axis***\
    **CLI Short:** ***-U***

    **Default value:** Y+

    **Valid values:** X-, Y-, Z-, X+, Y+, Z+

    Input scene up axis.

  - **forward-axis**

    **CLI Long:** ***--forward-axis***\
    **CLI Short:** ***-F***

    **Default value:** Z+

    **Valid values:** X-, Y-, Z-, X+, Y+, Z+

    Input scene forward axis.

  - **right-handed**

    **CLI Long:** ***--right-handed***\
    **CLI Short:** ***-R***

    **Default value:** true

    Input scene uses right handed coordiante system. (Finicky with root motion rotations)
