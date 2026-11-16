# Alpha Mini Built-in Actions Catalog

This catalog documents the complete, uncompressed, and verbatim reference of all **37 Built-in Actions** supported natively by the **Alpha Mini Robot** firmware, as defined in the official UBTECH AlphaMini Python SDK documentation (`docs-ubtrobot-alphamini-python-sdk-en-additional-html.pdf`).

These actions are executed programmatically using `PlayAction(action_name=...)` from the `mini.apis.api_action` module.

---

## Complete Built-in Actions Reference Table

| Index | Basic Action Name (Code) | Action Description | Can Interrupt? | Kinematic Scope & Usage Notes |
| :---: | :--- | :--- | :---: | :--- |
| 1 | `009` | Reset | **Yes** | Returns robot to default upright neutral standing posture |
| 2 | `012` | Push-ups | **No** | Full-body prone push-up exercise cycle; non-interruptible |
| 3 | `016` | Golden Rooster Independent | **No** | Dynamic single-leg balance pose; non-interruptible |
| 4 | `024` | Yoga | **No** | Multi-joint yoga balance routine; non-interruptible |
| 5 | `010` | Laughing | **Yes** | Upper torso and head laughter body movement |
| 6 | `random_short2` | Hug | **Yes** | Arms forward embrace / hugging motion |
| 7 | `027` | Sit down | **No** | Bipedal knee flexion into sitting position; non-interruptible |
| 8 | `031` | Squat | **No** | Deep squat posture; non-interruptible |
| 9 | `021` | Bent over | **No** | Torso forward bend; non-interruptible |
| 10 | `013` | Kung Fu | **No** | Martial arts dynamic stance; non-interruptible |
| 11 | `018` | Raise your right leg | **No** | Single-leg balance lift (Right); non-interruptible |
| 12 | `019` | Raise left leg | **No** | Single-leg balance lift (Left); non-interruptible |
| 13 | `017` | Raise your hands | **Yes** | Dual arm overhead raise |
| 14 | `015` | Welcome | **Yes** | Welcoming gesture / greeting |
| 15 | `011` | Nodding | **Yes** | Affirmative head nod |
| 16 | `random_short3` | Waving left hand | **Yes** | Left arm waving gesture |
| 17 | `random_short4` | Waving right hand | **Yes** | Right arm waving gesture |
| 18 | `028` | Right Lunge | **Yes** | Right forward leg lunge |
| 19 | `037` | Shaking his head | **Yes** | Negative / disagreement head shake |
| 20 | `038` | Tilt head | **Yes** | Inquisitive lateral head tilt |
| 21 | `Surveillance_001` | Say hello | **Yes** | Friendly greeting gesture |
| 22 | `Surveillance_003` | Handshake | **Yes** | Arm extension for human handshake |
| 23 | `Surveillance_004` | Blow kisses | **Yes** | Affectionate kissing arm and head gesture |
| 24 | `Surveillance_006` | Selling cute | **Yes** | Playful companion pose |
| 25 | `007` | Sit and stand | **No** | Transitions from sitting to upright standing |
| 26 | `action_014` | Invite | **Yes** | Welcoming directional hand gesture |
| 27 | `action_016` | Goodbye | **Yes** | Parting farewell wave |
| 28 | `action_012` | Seeking hug | **Yes** | Open-arms posture requesting a hug |
| 29 | `action_004` | Wow | **Yes** | Surprised whole-body gesture |
| 30 | `action_005` | Like | **Yes** | Thumbs-up / approval gesture |
| 31 | `action_006` | OK | **Yes** | Agreement gesture |
| 32 | `action_018` | Hey ha | **Yes** | Energetic martial-arts exclamation pose |
| 33 | `action_020` | 喝彩 (Cheer / Acclaim) | **Yes** | Jubilant cheering and celebration gesture |
| 34 | `action_011` | Pretend to fly | **Yes** | Arms outstretched airplane / flying motion |
| 35 | `action_013` | Make faces | **Yes** | Playful teasing gesture |
| 36 | `action_015` | Ass Twist | **Yes** | Torso and waist rhythmic wiggle |
| 37 | `action_007` | Kill you | **Yes** | Dramatic action sequence pose |
| 38 | `action_019` | Hold your head | **Yes** | Distressed hands-on-head pose |

---

## Programming with Actions: Python Snippet

```python
import asyncio
from mini.apis.api_action import PlayAction, GetActionList, RobotActionType
from test_connect import test_connect, shutdown, test_get_device_by_name

async def execute_action_sequence():
    # 1. Reset posture
    await PlayAction(action_name="009").execute()
    
    # 2. Perform a greeting gesture (Can interrupt)
    await PlayAction(action_name="Surveillance_001").execute()
    
    # 3. Perform push-ups (Cannot interrupt)
    await PlayAction(action_name="012").execute()
    
    # 4. Return to neutral standing
    await PlayAction(action_name="009").execute()
```

---

## Inter-Node Connectivity
- Action API: [[AlphaMiniMotionAndLocomotionAPI]]
- Dances Catalog: [[AlphaMiniBuiltInDancesCatalog]]
- Hardware Joint Specs: [[AlphaMiniHardwareSpecifications]]
- Lab 3 Protocol: [[Lab03-AlphaMiniBasicsAndChallengeProtocol]]

---

## Source Excerpt

> **Verbatim Excerpt from Pages 1 & 2, `docs-ubtrobot-alphamini-python-sdk-en-additional-html.pdf`:**
> *"Built-in actions: Basic action name | Action description | Can interrupt*
> *009 | Reset | Yes*
> *012 | Push-ups | No*
> *016 | Golden Rooster Independent | No*
> *024 | Yoga | No*
> *010 | Laughing | Yes*
> *random_short2 | hug | Yes*
> *027 | Sit down | No*
> *031 | Squat | No*
> *021 | Bent over | No*
> *013 | Kung Fu | No*
> *018 | Raise your right leg | No*
> *019 | Raise left leg | No*
> *017 | Raise your hands | Yes*
> *015 | Welcome | Yes*
> *011 | Nodding | Yes*
> *random_short3 | Waving left hand | Yes*
> *random_short4 | Waving right hand | Yes*
> *028 | Right Lunge | Yes*
> *037 | Shaking his head | Yes*
> *038 | tilt head | Yes*
> *Surveillance_001 | Say hello | Yes*
> *Surveillance_003 | Handshake | Yes*
> *Surveillance_004 | Blow kisses | Yes*
> *Surveillance_006 | Selling cute | Yes*
> *007 | Sit and stand | No*
> *action_014 | Invite | Yes*
> *action_016 | Goodbye | Yes*
> *action_012 | Seeking hug | Yes*
> *action_004 | Wow | Yes*
> *action_005 | Like | Yes*
> *action_006 | OK | Yes*
> *action_018 | Hey ha | Yes*
> *action_020 | 喝彩 | Yes*
> *action_011 | Pretend to fly | Yes*
> *action_013 | Make faces | Yes*
> *action_015 | Ass Twist | Yes*
> *action_007 | Kill you | Yes*
> *action_019 | Hold your head | Yes"*
