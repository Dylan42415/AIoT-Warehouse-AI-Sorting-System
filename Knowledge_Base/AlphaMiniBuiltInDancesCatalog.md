# Alpha Mini Built-in Dances Catalog

This catalog documents the complete, uncompressed reference of all **19 Built-in Dance Routines** supported natively by the **Alpha Mini Robot** firmware, as defined in the official UBTECH AlphaMini Python SDK documentation (`docs-ubtrobot-alphamini-python-sdk-en-additional-html.pdf`).

These dances synchronize 14-joint bipedal choreography, musical audio playback, and emotive LCD eye graphics using `StartBehavior(name=...)` from `mini.apis.api_behavior`.

---

## Complete Built-in Dances Reference Table

| Index | Dance Name (Identifier) | Action / Dance Description | Musical & Choreographic Profile |
| :---: | :--- | :--- | :--- |
| 1 | `w_stand_0001` | Hiccup | Humorous standing hiccup jerking motion with audio |
| 2 | `w_stand_0002` | Fart | Playful sound effect accompanied by comedic posture |
| 3 | `w_stand_0003` | Stretching | Morning wake-up arm and torso stretching routine |
| 4 | `w_stand_0008` | Sneeze | Pre-sneeze buildup and sudden sneezing torso flex |
| 5 | `w_stand_0009` | Tickle | Sensitive giggling and squirming body reaction |
| 6 | `w_stand_0010` | Scare | Playful spooky surprise posture |
| 7 | `dance_0001en` | Healthy Song and Dance | Full-body aerobic fitness musical choreography |
| 8 | `dance_0002en` | Shaolin Hero Dance | Kung-fu inspired martial arts dynamic routine |
| 9 | `dance_0003en` | Youth Training Manual Dance | Pop choreography with synchronized rhythmic arm swings |
| 10 | `dance_0004en` | Little Star | Gentle nursery melody with synchronized limb dance |
| 11 | `dance_0005en` | Grass Dance | Swaying nature choreography with rhythmic balance |
| 12 | `dance_0006en` | Seaweed Dance | Viral undulating wave motion through torso and arms |
| 13 | `dance_0007en` | I love taking a bath | Comedic shower choreography with scrubbing gestures |
| 14 | `dance_0008en` | Chongerfei | Melodic lullaby choreography with gentle posture shifts |
| 15 | `dance_0009en` | Learn to Meow | Playful feline-inspired dance with ear/paw gestures |
| 16 | `dance_0011en` | Dura Dance | Fast-paced Latin pop musical choreography |
| 17 | `dance_0013` | Buddha Girl | Popular rhythmic dance choreography |
| 18 | `custom_0035` | Happy birthday | Birthday celebration routine with song and clapping |
| 19 | `014` | Tai Chi | Traditional meditative slow martial balance routine |

---

## Programming with Dances: Python Snippet

```python
import asyncio
from mini.apis.api_behavior import StartBehavior, StopBehavior
from test_connect import test_connect, shutdown, test_get_device_by_name

async def perform_dance_demonstration():
    # 1. Execute 'Little Star'
    print("Starting Little Star dance...")
    await StartBehavior(name="dance_0004en").execute()
    
    # 2. Execute 'Tai Chi'
    print("Starting Tai Chi routine...")
    await StartBehavior(name="014").execute()
```

---

## Inter-Node Connectivity
- Behaviors API: [[AlphaMiniBehaviorsAndDanceAPI]]
- Actions Catalog: [[AlphaMiniBuiltInActionsCatalog]]
- Emoticons Catalog: [[AlphaMiniBuiltInEmoticonsCatalog]]
- Lab 3 Protocol: [[Lab03-AlphaMiniBasicsAndChallengeProtocol]]

---

## Source Excerpt

> **Verbatim Excerpt from Page 1, `docs-ubtrobot-alphamini-python-sdk-en-additional-html.pdf`:**
> *"Built-in dance: Dance name | Action description*
> *w_stand_0001 | Hiccup*
> *w_stand_0002 | Fart*
> *w_stand_0003 | Stretching*
> *w_stand_0008 | Sneeze*
> *w_stand_0009 | Tickle*
> *w_stand_0010 | Scare*
> *dance_0001en | Healthy Song and Dance*
> *dance_0002en | Shaolin Hero Dance*
> *dance_0003en | Youth Training Manual Dance*
> *dance_0004en | Little Star*
> *dance_0005en | Grass Dance*
> *dance_0006en | Seaweed Dance*
> *dance_0007en | I love taking a bath*
> *dance_0008en | Chongerfei*
> *dance_0009en | Learn to Meow*
> *dance_0011en | Dura Dance*
> *dance_0013 | Buddha Girl*
> *custom_0035 | Happy birthday*
> *014 | Tai Chi"*
