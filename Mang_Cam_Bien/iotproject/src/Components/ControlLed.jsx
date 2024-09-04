import React, { useState } from 'react';
import Button from '@mui/material/Button';
import Box from '@mui/material/Box';
import Card from '@mui/material/Card';
import CardContent from '@mui/material/CardContent';
import Typography from '@mui/material/Typography';
import LightbulbIcon from '@mui/icons-material/Lightbulb';

export default function LEDControl() {
  const [ledState, setLedState] = useState(false);

  const handleToggle = () => {
    setLedState(!ledState);
  };

  return (
    <Card style={{ margin: '10px', textAlign: 'center', height: '90%' }}>
      <CardContent>
        <Typography variant="h6" gutterBottom>
          Điều khiển đèn LED
        </Typography>
        <Box display="flex" flexDirection="column" alignItems="center">
          <LightbulbIcon style={{ fontSize: 80, color: ledState ? '#ffeb3b' : '#9e9e9e' }} />
          <Button 
            variant="contained" 
            color={ledState ? "primary" : "secondary"} 
            onClick={handleToggle} 
            style={{ padding: '12px 24px', marginTop: '20px' }}
          >
            {ledState ? "Tắt đèn LED" : "Bật đèn LED"}
          </Button>
        </Box>
      </CardContent>
    </Card>
  );
}
