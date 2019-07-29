/******************************************************************************\
* Copyright (C) Leap Motion, Inc. 2011-2014.                                   *
* Leap Motion proprietary. Licensed under Apache 2.0                           *
* Available at http://www.apache.org/licenses/LICENSE-2.0.html                 *
\******************************************************************************/

using UnityEngine;
using System.Collections.Generic;
using Leap;

// Overall Controller object that will instantiate hands when they appear.
public class HandControllerPlayer : HandController
{
    private bool show_hands_ = true;
    private long prev_graphics_id_ = 0;
    private long prev_physics_id_ = 0;

    void OnDrawGizmos()
    {
        // Draws the little Leap Motion Controller in the Editor view.
        Gizmos.matrix = Matrix4x4.Scale(GIZMO_SCALE * Vector3.one);
        //Gizmos.DrawIcon(transform.position, "leap_motion.png");
    }

    void Start()
    {
        leap_controller_ = null; // Only playback

        // Initialize hand lookup tables.
        hand_graphics_ = new Dictionary<int, HandModel>();
        hand_physics_ = new Dictionary<int, HandModel>();

        if (leap_controller_ == null)
        {
            Debug.Log("Player mode.");
        }

        recorder_.Stop();
        if (recordingAsset != null)
        {
            recorder_.Load(recordingAsset);
            Debug.Log("Loaded asset.");
            recorder_.Play();
        }
    }

    new public Frame GetFrame()
    {
        if (recorder_.state == RecorderState.Playing)
            return recorder_.GetCurrentFrame();

        return leap_controller_.Frame();
    }

    void Update()
    {
        UpdateRecorder();
        Frame frame = GetFrame();

        if (Input.GetKeyDown(KeyCode.H))
        {
            show_hands_ = !show_hands_;
        }

        if (show_hands_)
        {
            if (frame.Id != prev_graphics_id_)
            {
                UpdateHandModels(hand_graphics_, frame.Hands, leftGraphicsModel, rightGraphicsModel);
                prev_graphics_id_ = frame.Id;
            }
        }
        else
        {
            // Destroy all hands with defunct IDs.
            List<int> hands = new List<int>(hand_graphics_.Keys);
            for (int i = 0; i < hands.Count; ++i)
            {
                DestroyHand(hand_graphics_[hands[i]]);
                hand_graphics_.Remove(hands[i]);
            }
        }
    }

    void FixedUpdate()
    {
        if (leap_controller_ == null)
            return;

        Frame frame = GetFrame();

        if (frame.Id != prev_physics_id_)
        {
            UpdateHandModels(hand_physics_, frame.Hands, leftPhysicsModel, rightPhysicsModel);
            prev_physics_id_ = frame.Id;
        }
    }

    protected void UpdateRecorder()
    {
        recorder_.speed = recorderSpeed;
        recorder_.loop = recorderLoop;

        recorder_.NextFrame();
    }
}
