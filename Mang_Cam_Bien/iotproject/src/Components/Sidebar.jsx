import React from 'react';
import { Link } from 'react-router-dom';
import { Drawer, List, ListItem, ListItemText, ListItemIcon } from '@mui/material';
import DashboardIcon from '@mui/icons-material/Dashboard'; //take icon
import AccountCircleIcon from '@mui/icons-material/AccountCircle';
import SensorsIcon from '@mui/icons-material/Sensors';
import HistoryIcon from '@mui/icons-material/History';

export default function Sidebar() {
    return (
      <Drawer
        variant="permanent"
        sx={{
          '& .MuiDrawer-paper': {
            backgroundColor: '#1e3a8a', // Màu nền của Sidebar
            color: '#fff', // Màu chữ
          },
        }}
      >
        <List>
          <ListItem button component={Link} to="/">
            <ListItemIcon>
              <DashboardIcon style={{ color: '#fff' }} /> {/*take icon*/}
            </ListItemIcon>
            <ListItemText primary="Dashboard" />
          </ListItem>
          <ListItem button component={Link} to="/profile">
            <ListItemIcon>
              <AccountCircleIcon style={{ color: '#fff' }} />
            </ListItemIcon>
            <ListItemText primary="Profile" />
          </ListItem>
          <ListItem button component={Link} to="/data-sensor">
            <ListItemIcon>
              <SensorsIcon style={{ color: '#fff' }} />
            </ListItemIcon>
            <ListItemText primary="Data Sensor" />
          </ListItem>
          <ListItem button component={Link} to="/action-history">
            <ListItemIcon>
              <HistoryIcon style={{ color: '#fff' }} />
            </ListItemIcon>
            <ListItemText primary="Action History" />
          </ListItem>
        </List>
      </Drawer>
    );
}