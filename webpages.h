
const char General_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
    <style>
        .slider-container {
            display: flex;
            align-items: center;
        }

        .slider {
            width: 80%;
        }

        .slider-value {
            width: 20%;
            text-align: center;
        }

        table {
            width: 98%;
            border-collapse: collapse;
        }

        td {
            border: 1px solid black;
            padding: 1%; /* Adjust padding as needed */
        }

        .slider-label {
            text-align: center;
        }

        /* Adjust color-picker width and height to fill the entire cell */
        input[type="color"] {
            width: 100%;
            height: 100px;
        }
    </style>
</head>
<body>
<font size="12">
<h2 style="text-align: left">Therapy Light</h2>
<br>
<h3 style="color:red;">@@error_msg@@</h3>
<font size="5">

<FORM method="post" action="/params">
  <table>
      <tr>
          <td>
              Session Duration:
          </td>
          <td>
              <div class="slider-container">
                  <input type="range" id="mySlider" name="Webpage_TX_Duration" class="slider" min="0" max="120" step="5" oninput="updateSliderValue()" value="@@Webpage_Duration@@">
                  <div class="slider-value" id="sliderValue" ></div>Minutes
              </div>

              <script>
                  function updateSliderValue() {
                      const slider = document.getElementById('mySlider');
                      const sliderValue = document.getElementById('sliderValue');

                      sliderValue.textContent = slider.value;
                  }

                  // Initial update
                  updateSliderValue();
              </script>
          </td>
      </tr>
      <tr>        
          <td colspan="2">
          </td>
      </tr>
      <tr>
          <td>
              Time
          </td>
          <td>
              Color Selection
          </td>
      </tr>
      <tr>
          <td>
              Starting:
          </td>
          <td>
              <input type="color" id="color-picker1" name="Webpage_TX_Hex_Color1" onchange="updateColor()" value="@@Webpage_Hex_Color1@@">
          </td>
      </tr>
      <tr>
          <td>
              Half Time:
          </td>
          <td>
              <input type="color" id="color-picker2" name="Webpage_TX_Hex_Color2" onchange="updateColor()" value="@@Webpage_Hex_Color2@@">
          </td>
      </tr>
      <tr>
          <td>
              5 Minutes Remaining:
          </td>
          <td>
              <input type="color" id="color-picker3" name="Webpage_TX_Hex_Color3" onchange="updateColor()" value="@@Webpage_Hex_Color3@@">
          </td>
      </tr>
      <tr>
          <td>
              Overtime (Blinks):
          </td>
          <td>
              <input type="color" id="color-picker4" name="Webpage_TX_Hex_Color4" onchange="updateColor()" value="@@Webpage_Hex_Color4@@">
          </td>
        </tr>
        <TR>
          <TD>
            Password: 
          </TD>
          <td><input type="password" name="Login_Pass" style="font-size: 28px; width: 98%" maxlength="30"></td>
          </TD>
        </TR>
        <TR>
          <td colspan="2">
            <div style="text-align: right;">
              <input type="submit" value="Save Settings" style="font-size: 28px;">
            </div>
          </TD>
        </TR>
  </table>
</form>
<br>

<form method='POST' action="/reset">  
  <input type="submit" value="Factory Reset">
</form>
</body>
</html>

)=====";


