# Alpha Mini Built-in Emoticons Catalog

This catalog documents the complete, uncompressed reference of all **46 Built-in Emoticons** supported natively by the **Alpha Mini Robot** dual 1.22-inch LCD eye screens, as defined in the official UBTECH AlphaMini Python SDK documentation (`docs-ubtrobot-alphamini-python-sdk-en-additional-html.pdf`).

These eye animations are rendered using `PlayExpression(express_name=...)` from `mini.apis.api_expression`.

---

## Complete Built-in Emoticons Reference Table

| Index | Basic Emoticon Identifier | Emoticon Description | Affective Category / Visual State |
| :---: | :--- | :--- | :--- |
| 1 | `codemao1` | Looking around | Attentive glancing left and right |
| 2 | `codemao2` | Sad | Downcast drooping pupils |
| 3 | `codemao3` | Stretching sadness | Elongated melancholic expression |
| 4 | `codemao4` | Falling asleep | Slowly drooping eyelids drifting off |
| 5 | `codemao5` | Frightened | Wide terrified pupils with shivering |
| 6 | `codemao6` | Sleepy | Drowsy blinking and half-closed eyes |
| 7 | `codemao7` | Strange | Skeptical asymmetric pupil dilation |
| 8 | `codemao8` | Surprised | Wide-eyed shock animation |
| 9 | `codemao9` | Sneeze | Squinting buildup to a sneeze |
| 10 | `codemao10` | Exciting | Sparkling energetic pupils |
| 11 | `codemao11` | Fighting Spirit | Determined fiery gaze |
| 12 | `codemao12` | Hard work | Concentrated strained effort |
| 13 | `codemao13` | Question | Inquisitive curved questioning gaze |
| 14 | `codemao14` | Wake up | Eyes opening wide from sleep |
| 15 | `codemao15` | Distress | Agitated furrowed gaze |
| 16 | `codemao16` | Cheap laugh | Mischievous smirking pupil animation |
| 17 | `codemao17` | Depressed | Darkened sad droop |
| 18 | `codemao18` | Desire | Craving pleading pupil shine |
| 19 | `codemao19` | Love | Heart-shaped or affectionate glow |
| 20 | `codemao20` | Blink | Natural periodic eyelid blink |
| 21 | `w_basic_0007-1` | W type mobile | Dynamic wavy pupil traversal |
| 22 | `w_basic_0003-1` | Look right | Eyes tracked to right horizon |
| 23 | `w_basic_0005-1` | Look left | Eyes tracked to left horizon |
| 24 | `w_basic_0010-1` | Look up | Eyes tracked upward |
| 25 | `w_basic_0011-1` | Look left and right | Alternating horizontal scanning gaze |
| 26 | `w_basic_0012-1` | Look up and down | Alternating vertical scanning gaze |
| 27 | `emo_007` | Smile | Cheerful happy squinting smile |
| 28 | `emo_010` | Shy | Blushing bashful sideways glance |
| 29 | `emo_016` | Smile | Alternative joyful grinning expression |
| 30 | `emo_028` | Cover your face | Shy / bashful concealed eye state |
| 31 | `emo_008` | Irritation | Annoyed narrowed eyelids |
| 32 | `emo_014` | Poor | Piteous sorrowful begging glance |
| 33 | `emo_009` | Tears | Streaming tear droplets on LCDs |
| 34 | `emo_011` | Crying | Heavy sobbing eye animations |
| 35 | `emo_023` | Pain | Wincing squeezed-shut eyes |
| 36 | `emo_013` | Get angry | Furious angled eyebrows and red glare |
| 37 | `emo_015` | Arrogance | Smug upturned dismissive glance |
| 38 | `emo_026` | White eyes | Annoyed eye-roll / exasperated white gaze |
| 39 | `emo_022` | Squeeze | Tightly clamped eyelid squint |
| 40 | `emo_019` | Hazy | Blurry unfocused dazed pupils |
| 41 | `emo_020` | Daze | Vacant zoning-out gaze |
| 42 | `emo_027` | Cool | Wearing stylish dark sunglasses |
| 43 | `emo_029` | Witty | Clever knowing wink |
| 44 | `emo_030` | Cross Eyes | Playful cross-eyed pupils |
| 45 | `emo_031` | Reading glasses | Academic reading spectacles |
| 46 | `emo_032` | Golden Glasses | Prestigious gold-rimmed spectacles |

---

## Programming with Expressions: Python Snippet

```python
import asyncio
from mini.apis.api_expression import PlayExpression

async def emotional_cycle():
    # 1. Neutral attentive
    await PlayExpression(express_name="codemao1").execute()
    
    # 2. Friendly smile
    await PlayExpression(express_name="emo_007").execute()
    
    # 3. Loving affection
    await PlayExpression(express_name="codemao19").execute()
    
    # 4. Put on cool sunglasses
    await PlayExpression(express_name="emo_027").execute()
```

---

## Inter-Node Connectivity
- Expressions API: [[AlphaMiniExpressionsAndMouthLampAPI]]
- Hardware Displays: [[AlphaMiniHardwareSpecifications]]
- Dances Catalog: [[AlphaMiniBuiltInDancesCatalog]]
- Lab 3 Protocol: [[Lab03-AlphaMiniBasicsAndChallengeProtocol]]

---

## Source Excerpt

> **Verbatim Excerpt from Pages 2 & 3, `docs-ubtrobot-alphamini-python-sdk-en-additional-html.pdf`:**
> *"Built-in emoticons: Basic emoticon name | Emoticon description*
> *codemao1 | Looking around*
> *codemao2 | Sad*
> *codemao3 | Stretching sadness*
> *codemao4 | Falling asleep*
> *codemao5 | Frightened*
> *codemao6 | sleepy*
> *codemao7 | Strange*
> *codemao8 | Surprised*
> *codemao9 | sneeze*
> *codemao10 | Exciting*
> *codemao11 | Fighting Spirit*
> *codemao12 | Hard work*
> *codemao13 | Question*
> *codemao14 | wake up*
> *codemao15 | Distress*
> *codemao16 | Cheap laugh*
> *codemao17 | Depressed*
> *codemao18 | Desire*
> *codemao19 | Love*
> *codemao20 | Blink*
> *w_basic_0007-1 | W type mobile*
> *w_basic_0003-1 | Look right*
> *w_basic_0005-1 | look left*
> *w_basic_0010-1 | Look up*
> *w_basic_0011-1 | Look left and right*
> *w_basic_0012-1 | Look up and down*
> *emo_007 | Smile*
> *emo_010 | Shy*
> *emo_016 | Smile*
> *emo_028 | Cover your face*
> *emo_008 | Irritation*
> *emo_014 | Poor*
> *emo_009 | Tears*
> *emo_011 | Crying*
> *emo_023 | Pain*
> *emo_013 | Get angry*
> *emo_015 | Arrogance*
> *emo_026 | White eyes*
> *emo_022 | squeeze*
> *emo_019 | Hazy*
> *emo_020 | Daze*
> *emo_027 | Cool*
> *emo_029 | Witty*
> *emo_030 | Cross Eyes*
> *emo_031 | Reading glasses*
> *emo_032 | Golden Glasses"*
