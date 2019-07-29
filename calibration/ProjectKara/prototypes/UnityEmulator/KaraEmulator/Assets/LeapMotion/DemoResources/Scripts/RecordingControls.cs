using UnityEngine;
using System.Collections;

public class RecordingControls : MonoBehaviour
{

    public KeyCode recordSequence = KeyCode.R;
    public KeyCode recordFrame = KeyCode.F;
    public KeyCode finishAndSave = KeyCode.S;
    public KeyCode resetRecording = KeyCode.Space;

    void Update()
    {
        HandController controller = GetComponent<HandController>();
        if (Input.GetKeyDown(recordSequence))
            controller.Record();

        if (Input.GetKeyDown(recordFrame))
            controller.RecordFrame();

        if (Input.GetKeyDown(finishAndSave))
        {
            Debug.Log("Recording saved to: " + controller.FinishAndSave());
        }

        if (Input.GetKeyDown(resetRecording))
            controller.ResetRecording();
    }
}
