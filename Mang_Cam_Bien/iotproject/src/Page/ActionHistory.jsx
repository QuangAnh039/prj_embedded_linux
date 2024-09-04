import React, { useState, useEffect } from 'react';
import { Box, Typography, Paper, List, ListItem, ListItemText } from '@mui/material';

export default function ActionHistory() {
  const [history, setHistory] = useState([]);

  useEffect(() => {
    setTimeout(() => {
      setHistory([
        { id: 1, action: 'LED turned ON', timestamp: '2024-08-27 14:00' },
        { id: 2, action: 'LED turned OFF', timestamp: '2024-08-27 14:30' },
        { id: 3, action: 'Sensor threshold updated', timestamp: '2024-08-27 15:00' },
      ]);
    }, 2000);
  }, []);

  return (
    <Box padding="20px" marginLeft="250px">
      <Paper elevation={3} style={{ padding: '20px' }}>
        <Typography variant="h4" gutterBottom>
          Action History
        </Typography>
        <List>
          {history.map((entry) => (
            <ListItem key={entry.id}>
              <ListItemText primary={`${entry.action} at ${entry.timestamp}`} />
            </ListItem>
          ))}
        </List>
      </Paper>
    </Box>
  );
}
