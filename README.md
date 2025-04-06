# Utility AI Plugin
An Unreal Engine plugin for AI decision making through modular utilities and consideration scoring.

## Features

- `UtilityAIAction` defines a possible choice. Calculate a score from 0..1 to determine when it's best to take this action.
- Define and debug individual scoring elements, e.g. distance to target, speed of approach, time since last action taken.
- Group actions in a `UtilityAIActionSet` and define scoring factors for each to create relative differences in priority.
- Filter actions by gameplay tags, built to work with ability system.
- Actions can run any logic, but `UtilityAIBehaviorAction` can easily run single-purpose easier-to-design behavior trees.
- Control when action scores are frozen, or changing actions is forbidden, with tag-based interruption rules.

At its core, actions make up each possible branch or decision an AI can make, which are very similar to abilities or single behaviors. Designing the execution of those actions is easy to do with existing tools like behavior trees or gameplay abilities, but organizing the decision making layer is difficult and tedious.

Utility AI allows you to drop all possible actions into a bucket for the AI to consider, then design organic considerations that react and respond to various elements of the world. Is a character approaching fast? Are we taking too much damage too quickly? How long has it been since we last ran this action? What is our relative position / view angle / line-of-sight? Each of these can represent individual scores, combined together to form a more complete score for an action.

When an action achieves a higher score then all the rest, it's activated. While an action is active it can freeze its score to prevent it from dropping and being out-selected again, or even apply tags that indicate the AI is busy and cannot change actions.

A visualization of all the actions and their scoring elements are shown in Gameplay Debugger, so you can easily tweak relative scoring factors to make decisions feel informed and natural.
